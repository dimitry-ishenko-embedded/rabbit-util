////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "message.hpp"
#include "rabbit.hpp"
#include "serial.hpp"

#include <chrono>
#include <cstdint>
#include <numeric> // std::accumulate
#include <sstream>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;
using namespace std::this_thread;

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context& ctx, const std::string& name)
{
    asio::serial_port serial{ctx};
    do_("Opening serial port ", name, [&]{ serial = asio::serial_port{ctx, name}; });
    return serial;
}

////////////////////////////////////////////////////////////////////////////////
namespace
{

// ioi ld (WDTTR), 0x51
// ioi ld (WDTTR), 0x54
constexpr char disable_wd[] = "\x80\x09\x51\x80\x09\x54";

// ioi ld (GOCR), 0x30
constexpr char status_hi[] = "\x80\x0e\x30";

// ioi ld (GOCR), 0x20
constexpr char status_lo[] = "\x80\x0e\x20";

void send_file(asio::serial_port& serial, const payload& data, std::size_t max_size = 0)
{
    if (max_size == 0) max_size = data.size();
    else if (max_size > data.size()) max_size = data.size();

    asio::write(serial, asio::buffer(data), [&](const asio::error_code& ec, std::size_t n){
        auto pc = n * 100 / max_size;
        message(pc, "%... ", std::string(5 + ((pc < 10) ? 1 : (pc < 100) ? 2 : 3), '\b'));
        return max_size - n;
    });

    drain(serial);
    message("100%... ");
}

auto addressof(auto& val) { return reinterpret_cast<std::uint8_t*>(&val); }

#pragma pack(push, 1)
struct pilot_head
{
    std::uint32_t addr;
    std::uint16_t size;
    std::uint8_t  csum;
};
#pragma pack(pop)

auto to_hex(int val)
{
    std::ostringstream os;
    os << std::hex << val;
    return "0x" + std::move(os).str();
}

// https://en.wikipedia.org/wiki/Fletcher's_checksum#Implementation
auto fletcher16(const payload& data)
{
    std::uint16_t a = 0, b = 0;
    for (auto c : data) { a = (a + c) % 255; b = (b + a) % 255; }
    return b |= a << 8; // NB: Rabbit ordering
}

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

        // check the /STATUS pin
        sleep_for(100ms);
        if (dsr(serial)) throw std::runtime_error{"Target not responding"};

        // tell Rabbit to set the /STATUS pin low
        doing("L");
        asio::write(serial, asio::buffer(status_lo, size(status_lo)));
        drain(serial);

        // check the /STATUS pin
        sleep_for(100ms);
        if (!dsr(serial)) throw std::runtime_error{"Target not responding"};
    });
}

////////////////////////////////////////////////////////////////////////////////
void send_stage1(asio::serial_port& serial, const payload& coldload)
{
    do_("Sending initial loader", [&]{
        baud_rate(serial, 2400);
        send_file(serial, coldload);
        sleep_for(100ms);
    });
}

////////////////////////////////////////////////////////////////////////////////
void send_stage2(asio::serial_port& serial, const payload& pilot)
{
    do_("Sending secondary loader", [&]{
        baud_rate(serial, 57600);
        flush(serial, que_in);

        pilot_head head;
        head.addr = 0x4100;
        head.size = pilot.size();
        head.csum = std::accumulate(addressof(head), addressof(head) + sizeof(head) - sizeof(head.csum), 0);

        doing("H");
        asio::write(serial, asio::buffer( addressof(head), sizeof(head) ));
        drain(serial);

        doing("C");
        std::uint8_t csr;
        asio::read(serial, asio::buffer( addressof(csr), sizeof(csr) ));
        if (head.csum != csr) throw std::runtime_error{
            "Checksum error: ours=" + to_hex(head.csum) + " theirs=" + to_hex(csr)
        };

        send_file(serial, pilot);

        doing("C");
        std::uint16_t fsr;
        asio::read(serial, asio::buffer( addressof(fsr), sizeof(fsr) ));
        if (auto fsl = fletcher16(pilot); fsl != fsr) throw std::runtime_error{
            "Checksum error: ours=" + to_hex(fsl) + " theirs=" + to_hex(fsr)
        };
    });
}
