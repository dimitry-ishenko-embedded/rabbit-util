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

void baud_rate(asio::serial_port&, unsigned);
void baud_rate(asio::serial_port&, unsigned, asio::error_code&);

bool dsr(asio::serial_port&);
bool dsr(asio::serial_port&, asio::error_code&);

bool dtr(asio::serial_port&);
bool dtr(asio::serial_port&, asio::error_code&);

void dtr(asio::serial_port&, bool);
void dtr(asio::serial_port&, bool, asio::error_code&);

////////////////////////////////////////////////////////////////////////////////
#endif
