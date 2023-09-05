////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "message.hpp"
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

#pragma pack(push, 1)
struct pilot_head
{
    dword address;
    word size;
    byte check;
};
#pragma pack(pop)

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
