////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "serial_port.hpp"

#if defined(__unix__) || defined(__APPLE__)
  #include <cerrno>
  #include <termios.h>
#else
  #error "Unsupported platform"
#endif

////////////////////////////////////////////////////////////////////////////////
void serial_port::set(::baud_rate b) { set_option(asio::serial_port::baud_rate{b}); }
void serial_port::set(::baud_rate b, asio::error_code& ec) { set_option(asio::serial_port::baud_rate{b}, ec); }

void serial_port::set(::flow_control f) { set_option(asio::serial_port::flow_control{f}); }
void serial_port::set(::flow_control f, asio::error_code& ec) { set_option(asio::serial_port::flow_control{f}, ec); }

void serial_port::set(::parity p) { set_option(asio::serial_port::parity{p}); }
void serial_port::set(::parity p, asio::error_code& ec) { set_option(asio::serial_port::parity{p}, ec); }

void serial_port::set(::stop_bits s) { set_option(asio::serial_port::stop_bits{s}); }
void serial_port::set(::stop_bits s, asio::error_code& ec) { set_option(asio::serial_port::stop_bits{s}, ec); }

void serial_port::set(::char_size c) { set_option(asio::serial_port::character_size{c}); }
void serial_port::set(::char_size c, asio::error_code& ec) { set_option(asio::serial_port::character_size{c}, ec); }

////////////////////////////////////////////////////////////////////////////////
void serial_port::set_bit(int bit, bool s, asio::error_code& ec)
{
    int fd = native_handle();
    if (ioctl(fd, s ? TIOCMBIS : TIOCMBIC, &bit)) ec.assign(errno, asio::system_category());
}

bool serial_port::get_bit(int bit, asio::error_code& ec) const
{
    int fd = const_cast<serial_port*>(this)->native_handle();
    int bits = 0;
    if (ioctl(fd, TIOCMGET, &bits)) ec.assign(errno, asio::system_category());
    return (bits & bit);
}

////////////////////////////////////////////////////////////////////////////////
void serial_port::cts(bool st)
{
    asio::error_code ec;
    cts(st, ec);
    asio::detail::throw_error(ec, "cts");
}
void serial_port::cts(bool s, asio::error_code& ec) { set_bit(TIOCM_CTS, s, ec); }

bool serial_port::dcd() const
{
    asio::error_code ec;
    auto s = dcd(ec);
    asio::detail::throw_error(ec, "dcd");
    return s;
}
bool serial_port::dcd(asio::error_code& ec) const { return get_bit(TIOCM_CAR, ec); }

bool serial_port::dsr() const
{
    asio::error_code ec;
    auto s = dsr(ec);
    asio::detail::throw_error(ec, "dsr");
    return s;
}
bool serial_port::dsr(asio::error_code& ec) const { return get_bit(TIOCM_DSR, ec); }

void serial_port::dtr(bool st)
{
    asio::error_code ec;
    dtr(st, ec);
    asio::detail::throw_error(ec, "dtr");
}
void serial_port::dtr(bool s, asio::error_code& ec) { set_bit(TIOCM_DTR, s, ec); }

bool serial_port::dtr() const
{
    asio::error_code ec;
    auto s = dtr(ec);
    asio::detail::throw_error(ec, "dtr");
    return s;
}
bool serial_port::dtr(asio::error_code& ec) const { return get_bit(TIOCM_DTR, ec); }

bool serial_port::ri() const
{
    asio::error_code ec;
    auto s = ri(ec);
    asio::detail::throw_error(ec, "ri");
    return s;
}
bool serial_port::ri(asio::error_code& ec) const { return get_bit(TIOCM_RI, ec); }

void serial_port::rts(bool st)
{
    asio::error_code ec;
    rts(st, ec);
    asio::detail::throw_error(ec, "rts");
}
void serial_port::rts(bool s, asio::error_code& ec) { set_bit(TIOCM_RTS, s, ec); }

bool serial_port::rts() const
{
    asio::error_code ec;
    auto s = rts(ec);
    asio::detail::throw_error(ec, "rts");
    return s;
}
bool serial_port::rts(asio::error_code& ec) const { return get_bit(TIOCM_RTS, ec); }
