////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <asio.hpp>

enum { lo = false, hi = true };

inline void baud_rate(asio::serial_port& serial, unsigned rate)
{
    serial.set_option(asio::serial_port::baud_rate{rate});
}
inline void baud_rate(asio::serial_port& serial, unsigned rate, asio::error_code& ec)
{
    serial.set_option(asio::serial_port::baud_rate{rate}, ec);
}

bool dsr(asio::serial_port&);
bool dsr(asio::serial_port&, asio::error_code&);

bool dtr(asio::serial_port&);
bool dtr(asio::serial_port&, asio::error_code&);

void dtr(asio::serial_port&, bool);
void dtr(asio::serial_port&, bool, asio::error_code&);

void drain(asio::serial_port&);
void drain(asio::serial_port&, asio::error_code&);

enum que { que_in, que_out, que_both };
void flush(asio::serial_port&, que);
void flush(asio::serial_port&, que, asio::error_code&);

////////////////////////////////////////////////////////////////////////////////
#endif
