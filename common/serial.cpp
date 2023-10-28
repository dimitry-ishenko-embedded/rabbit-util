////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "message.hpp"
#include "serial.hpp"

#if defined(__unix__) || defined(__APPLE__)
  #include <cerrno>
  #include <termios.h>
#else
  #error "Unsupported platform"
#endif

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context& ctx, const std::string& name)
{
    asio::serial_port serial{ctx};
    do_("Opening serial port ", name, [&]{ serial = asio::serial_port{ctx, name}; });
    return serial;
}

////////////////////////////////////////////////////////////////////////////////
void send_data(asio::serial_port& serial, const payload& data, size_t max_size)
{
    if (max_size == 0) max_size = data.size();
    else if (max_size > data.size()) max_size = data.size();

    asio::write(serial, asio::buffer(data), [&](const asio::error_code& ec, size_t n){
        auto pc = n * 100 / max_size;
        message(pc, "%... ", std::string(5 + ((pc < 10) ? 1 : (pc < 100) ? 2 : 3), '\b'));
        return max_size - n;
    });

    drain(serial);
    message("100%... ");
}

////////////////////////////////////////////////////////////////////////////////
void baud_rate(asio::serial_port& serial, unsigned rate)
{
    serial.set_option(asio::serial_port::baud_rate{rate});
}

void baud_rate(asio::serial_port& serial, unsigned rate, asio::error_code& ec)
{
    serial.set_option(asio::serial_port::baud_rate{rate}, ec);
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

////////////////////////////////////////////////////////////////////////////////
bool cts(asio::serial_port& serial)
{
    asio::error_code ec;
    auto s = cts(serial, ec);
    asio::detail::throw_error(ec, "cts");
    return s;
}
bool cts(asio::serial_port& serial, asio::error_code& ec) { return get_bit(serial, TIOCM_CTS, ec); }

////////////////////////////////////////////////////////////////////////////////
bool rts(asio::serial_port& serial)
{
    asio::error_code ec;
    auto s = rts(serial, ec);
    asio::detail::throw_error(ec, "rts");
    return s;
}
bool rts(asio::serial_port& serial, asio::error_code& ec) { return get_bit(serial, TIOCM_RTS, ec); }

void rts(asio::serial_port& serial, bool s)
{
    asio::error_code ec;
    rts(serial, s, ec);
    asio::detail::throw_error(ec, "rts");
}
void rts(asio::serial_port& serial, bool s, asio::error_code& ec) { set_bit(serial, TIOCM_RTS, s, ec); }

////////////////////////////////////////////////////////////////////////////////
bool dsr(asio::serial_port& serial)
{
    asio::error_code ec;
    auto s = dsr(serial, ec);
    asio::detail::throw_error(ec, "dsr");
    return s;
}
bool dsr(asio::serial_port& serial, asio::error_code& ec) { return get_bit(serial, TIOCM_DSR, ec); }

////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
void flush(asio::serial_port& serial, que que)
{
    asio::error_code ec;
    flush(serial, que, ec);
    asio::detail::throw_error(ec, "flush");
}

void flush(asio::serial_port& serial, que que, asio::error_code& ec)
{
    int fd = serial.native_handle();
    int qs = (que == que_in) ? TCIFLUSH : (que == que_out) ? TCOFLUSH : (que == que_both) ? TCIOFLUSH : -1;
    if (tcflush(fd, qs)) ec.assign(errno, asio::system_category());
}
