////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "rabbit.hpp"

#include <chrono>
#include <cmath> // std::round
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;
using namespace std::this_thread;

////////////////////////////////////////////////////////////////////////////////
namespace
{

template<typename... Args>
inline void doing(Args&&... args) { (std::cout << ... << std::forward<Args>(args)) << "... " << std::flush; }

inline void done() { std::cout << "OK" << std::endl; }
inline void fail() { std::cout << "FAILED" << std::endl; }

inline auto to_human(unsigned n)
{
    std::ostringstream os;

    if (n < 1000) os << n << " bytes";
    else if (n < 999950) os << std::round(n / 100.) / 10. << " KB";
    else os << std::round(n / 100000.) / 10. << " MB";

    return std::move(os).str();
}

}

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context& ctx, const std::string& name)
try
{
    doing("Opening serial port ", name);
    asio::serial_port serial{ctx, name};

    done();
    return std::move(serial);
}
catch(...) { fail(); throw; }

////////////////////////////////////////////////////////////////////////////////
payload read_file(asio::io_context& ctx, const std::string& path)
try
{
    doing("Opening file ", path);
    asio::stream_file file{ctx, path, asio::stream_file::read_only};
    done();

    doing("Reading data");
    doing(to_human(file.size()));

    payload data;
    asio::read(file, asio::dynamic_buffer(data, file.size()));
    done();

    return data;
}
catch(...) { fail(); throw; }

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

}

////////////////////////////////////////////////////////////////////////////////
void reset_target(asio::serial_port& serial)
try
{
    doing("Resetting target");

    dtr(serial, hi);
    sleep_for(250ms);

    dtr(serial, lo);
    sleep_for(350ms);

    done();
}
catch(...) { fail(); throw; }

////////////////////////////////////////////////////////////////////////////////
void detect_target(asio::serial_port& serial)
try
{
    doing("Detecting presence");

    baud_rate(serial, 2400);

    // disable watchdog
    doing("W");
    asio::write(serial, asio::buffer(disable_wd, sizeof(disable_wd) - 1));

    // tell Rabbit to set the /STATUS pin high
    doing("H");
    asio::write(serial, asio::buffer(status_hi, sizeof(status_hi) - 1));
    sleep_for(100ms);
    // check the /STATUS pin
    if (dsr(serial)) throw std::runtime_error{"Target not responding"};

    // tell Rabbit to set the /STATUS pin low
    doing("L");
    asio::write(serial, asio::buffer(status_lo, sizeof(status_lo) - 1));
    sleep_for(100ms);
    // check the /STATUS pin
    if (!dsr(serial)) throw std::runtime_error{"Target not responding"};

    done();
}
catch(...) { fail(); throw; }
