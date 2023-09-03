////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "serial.hpp"

#if defined(__unix__) || defined(__APPLE__)
  #include <cerrno>
  #include <termios.h>
#else
  #error "Unsupported platform"
#endif

////////////////////////////////////////////////////////////////////////////////
void baud_rate(asio::serial_port& serial, unsigned b)
{
    serial.set_option(asio::serial_port::baud_rate{b});
}

void baud_rate(asio::serial_port& serial, unsigned b, asio::error_code& ec)
{
    serial.set_option(asio::serial_port::baud_rate{b}, ec);
}

////////////////////////////////////////////////////////////////////////////////
namespace
{

void set_bit(asio::serial_port& serial, int bit, bool s, asio::error_code& ec)
{
    int fd = serial.native_handle();
    if (ioctl(fd, s ? TIOCMBIS : TIOCMBIC, &bit)) ec.assign(errno, asio::system_category());
}

bool get_bit(asio::serial_port& serial, int bit, asio::error_code& ec)
{
    int fd = serial.native_handle();
    int bits = 0;
    if (ioctl(fd, TIOCMGET, &bits)) ec.assign(errno, asio::system_category());
    return (bits & bit);
}

}

bool dsr(asio::serial_port& serial)
{
    asio::error_code ec;
    auto s = dsr(serial, ec);
    asio::detail::throw_error(ec, "dsr");
    return s;
}
bool dsr(asio::serial_port& serial, asio::error_code& ec) { return get_bit(serial, TIOCM_DSR, ec); }

bool dtr(asio::serial_port& serial)
{
    asio::error_code ec;
    auto s = dtr(serial, ec);
    asio::detail::throw_error(ec, "dtr");
    return s;
}
bool dtr(asio::serial_port& serial, asio::error_code& ec) { return get_bit(serial, TIOCM_DTR, ec); }

void dtr(asio::serial_port& serial, bool s)
{
    asio::error_code ec;
    dtr(serial, s, ec);
    asio::detail::throw_error(ec, "dtr");
}
void dtr(asio::serial_port& serial, bool s, asio::error_code& ec) { set_bit(serial, TIOCM_DTR, s, ec); }

////////////////////////////////////////////////////////////////////////////////
void drain(asio::serial_port& serial)
{
    asio::error_code ec;
    drain(serial, ec);
    asio::detail::throw_error(ec, "drain");
}

void drain(asio::serial_port& serial, asio::error_code& ec)
{
    int fd = serial.native_handle();
    if (tcdrain(fd)) ec.assign(errno, asio::system_category());
}
