////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "message.hpp"
#include "raad.hpp"
#include "rabbit.hpp"
#include "serial.hpp"
#include "types.hpp"

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
namespace
{

// ioi ld (WDTTR), 0x51
// ioi ld (WDTTR), 0x54
constexpr byte disable_wd[] = "\x80\x09\x51\x80\x09\x54";

// ioi ld (GOCR), 0x30
constexpr byte status_hi[] = "\x80\x0e\x30";

// ioi ld (GOCR), 0x20
constexpr byte status_lo[] = "\x80\x0e\x20";

// ioi ld (SPCR), 0x80
constexpr byte start_pgm[] = "\x80\x24\x80";

}

////////////////////////////////////////////////////////////////////////////////
void reset_target(asio::serial_port& serial)
{
    do_("Resetting target", [&]{
        dtr(serial, hi);
        sleep_for(250ms);

        dtr(serial, lo);
        sleep_for(350ms);
    });
}

void detect_target(asio::serial_port& serial)
{
    do_("Detecting presence", [&]{
        baud_rate(serial, 2400);

        // disable watchdog
        doing("W");
        asio::write(serial, asio::buffer(disable_wd, size(disable_wd)));

        // tell Rabbit to set the /STATUS pin high
        doing("H");
        asio::write(serial, asio::buffer(status_hi, size(status_hi)));
        drain(serial);

        // check the /STATUS pin (inverted)
        sleep_for(100ms);
        if (dsr(serial)) throw std::runtime_error{"Target not responding"};

        // tell Rabbit to set the /STATUS pin low
        doing("L");
        asio::write(serial, asio::buffer(status_lo, size(status_lo)));
        drain(serial);

        // check the /STATUS pin (inverted)
        sleep_for(100ms);
        if (!dsr(serial)) throw std::runtime_error{"Target not responding"};
    });
}

////////////////////////////////////////////////////////////////////////////////
void send_coldload(asio::serial_port& serial, const payload& data)
{
    do_("Sending initial loader", [&]{
        baud_rate(serial, 2400);

        // send loader without the final triplet (see bootstrapping.md)
        send_data(serial, data, data.size() - 3);

        // tell Rabbit to set the /STATUS pin high
        doing("H");
        asio::write(serial, asio::buffer(status_hi, size(status_hi)));

        // send the final triplet
        doing("F");
        asio::write(serial, asio::buffer(start_pgm, size(start_pgm)));
        drain(serial);

        // check the /STATUS pin (inverted)
        sleep_for(100ms);
        if (dsr(serial)) throw std::runtime_error{"Target not responding"};
    });
}

////////////////////////////////////////////////////////////////////////////////
void send_pilot(asio::serial_port& serial, const payload& data)
{
    do_("Sending secondary loader", [&]{
        baud_rate(serial, 57600);
        flush(serial, que_in);

        pilot_head head;
        head.address = 0x4000;
        head.size = data.size();
        head.check = checksum(addressof(head), sizeof(head) - sizeof(head.check));

        doing("H");
        asio::write(serial, asio::buffer(addressof(head), sizeof(head)));
        drain(serial);

        doing("C");
        byte check;
        asio::read(serial, asio::buffer(addressof(check), sizeof(check)));
        if (head.check != check) throw std::runtime_error{
            "Checksum error: local=" + to_hex(head.check) + " remote=" + to_hex(check)
        };

        send_data(serial, data);
        auto fsl = fletcher16(data.data(), data.size());

        doing("C");
        word fsr;
        asio::read(serial, asio::buffer(addressof(fsr), sizeof(fsr)));
        if (fsl != fsr) throw std::runtime_error{
            "Checksum error: local=" + to_hex(fsl) + " remote=" + to_hex(fsr)
        };
    });
}

////////////////////////////////////////////////////////////////////////////////
namespace
{

void add_escaped(payload& packet, const byte* data, size_t size)
{
    // assume 10% of data needs to be escaped
    packet.reserve(packet.size() + size + size / 10);

    for (auto end = data + size; data != end; ++data)
        if (*data == TC_FRAMING_START || *data == TC_FRAMING_ESC)
        {
            packet.push_back(TC_FRAMING_ESC);
            packet.push_back(*data & ~0x20); // 7d -> 7d 5d, 7e -> 7d 5e
        }
        else packet.push_back(*data);
}

void send_packet(asio::serial_port& serial, byte subtype, const byte* data, size_t size)
{
    packet_head head;
    head.version    = TC_VERSION;
    head.flags      = 0;
    head.type       = TC_TYPE_SYSTEM;
    head.subtype    = subtype;
    head.data_size  = size;
    head.check      = fletcher16(addressof(head), sizeof(head) - sizeof(head.check));

    auto check = fletcher16(addressof(head), sizeof(head));
    check = fletcher16(check, data, size);

    payload packet{ TC_FRAMING_START };
    add_escaped(packet, addressof(head), sizeof(head));
    add_escaped(packet, data, size);
    add_escaped(packet, addressof(check), sizeof(check));

    asio::write(serial, asio::buffer(packet));
    drain(serial);
}
void send_packet(asio::serial_port& serial, byte subtype) { send_packet(serial, subtype, nullptr, 0); }

////////////////////////////////////////////////////////////////////////////////
auto read_escaped(asio::serial_port& serial, size_t size)
{
    payload packet(size);
    for (auto data = packet.data(), end = data + size; data != end; ++data)
    {
        byte c;
        asio::read(serial, asio::buffer(addressof(c), sizeof(c)));
        if (c == TC_FRAMING_ESC)
        {
            asio::read(serial, asio::buffer(addressof(c), sizeof(c)));
            *data = c | 0x20;
        }
        else *data = c;
    }
    return packet;
}

auto recv_packet(asio::serial_port& serial, byte subtype)
{
    for (;;)
    {
        byte c = 0;
        asio::read(serial, asio::buffer(addressof(c), sizeof(c)));
        if (c == TC_FRAMING_START)
        {
            auto chunk = read_escaped(serial, sizeof(packet_head));
            auto head = new (chunk.data()) packet_head;
            if (head->type == TC_TYPE_SYSTEM && (head->subtype & TC_SUBTYPE_MASK) == subtype)
            {
                bool is_ack = head->subtype & TC_ACK;
                auto payload = read_escaped(serial, head->data_size);

                auto chunk = read_escaped(serial, sizeof(word));
                auto fsr = new (chunk.data()) word;

                auto fsl = fletcher16(addressof(*head), sizeof(*head));
                fsl = fletcher16(fsl, payload.data(), payload.size());

                if (fsl != *fsr) throw std::runtime_error{
                    "Checksum error: local=" + to_hex(fsl) + " remote=" + to_hex(*fsr)
                };

                return std::tuple{is_ack, std::move(payload)};
            }
            else /* warning? */;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
auto find_baud_rate(asio::serial_port& serial)
{
    sleep_for(100ms);
    for (auto rate = max_baud_rate; rate >= min_baud_rate; rate /= 2)
    {
        doing(rate);
        send_packet(serial, TC_SYSTEM_SETBAUDRATE, addressof(rate), sizeof(rate));
        auto [is_ack, payload] = recv_packet(serial, TC_SYSTEM_SETBAUDRATE);

        if (is_ack) return rate;

        sleep_for(250ms);
    }
    throw std::runtime_error{"No suitable baud rate"};
}

auto recv_info(asio::serial_port& serial)
{
    sleep_for(100ms);
    send_packet(serial, TC_SYSTEM_INFOPROBE);
    auto [is_ack, payload] = recv_packet(serial, TC_SYSTEM_INFOPROBE);

    if (!is_ack) throw std::runtime_error{"Error getting info data"};
    if (payload.size() != sizeof(info_probe)) throw std::runtime_error{"Invalid info data"};

    auto info = new (payload.data()) info_probe;
    return *info;
}

void send_flash_data(asio::serial_port& serial, const flash_data& flash)
{
    sleep_for(100ms);
    send_packet(serial, TC_SYSTEM_FLASHDATA, addressof(flash.param), sizeof(flash.param));
    auto [is_ack, payload] = recv_packet(serial, TC_SYSTEM_FLASHDATA);

    if (!is_ack) throw std::runtime_error{"Error setting flash parameters"};
}

}

void send_program(asio::serial_port& serial, const payload& program)
{
    unsigned rate;
    do_("Negotiating baud rate", [&]{ rate = find_baud_rate(serial); });
    do_("Switching to ", rate, [&]{ baud_rate(serial, rate); });

    info_probe probe;
    do_("Probing board info", [&]{ probe = recv_info(serial); });

    message("CPU   ID: ", to_hex(probe.cpu_id));
    if (auto it = cpu_info.find(probe.cpu_id); it != cpu_info.end())
        message(" (", it->second, ")");
    message('\n');

    message("Board ID: ", to_hex(probe.id_block.prod_id));
    if (auto it = board_info.find(probe.id_block.prod_id); it != board_info.end())
        message(" (", it->second, ")");
    message('\n');

    auto it = flash_info.find(probe.flash_id);
    if (it == flash_info.end()) throw std::runtime_error{"Unsupported flash type " + to_hex(probe.flash_id)};

    auto& flash = it->second;
    message("Flash ID: ", to_hex(probe.flash_id), " (", flash.name, ")\n");
    message("  sec_size   = ", flash.param.sec_size,   '\n');
    message("  num_sec    = ", flash.param.num_sec,    '\n');
    message("  flash_size = ", flash.param.flash_size, '\n');
    message("  write_mode = ", flash.param.write_mode, '\n');

    do_("Sending flash data", [&]{ send_flash_data(serial, flash); });
}
