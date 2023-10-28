////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SERIAL_HPP
#define SERIAL_HPP

#include "types.hpp"

#include <asio.hpp>
#include <string>

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context&, const std::string& name);
void send_data(asio::serial_port&, const payload&, size_t max_size = 0);

void baud_rate(asio::serial_port&, unsigned);
void baud_rate(asio::serial_port&, unsigned, asio::error_code&);

bool cts(asio::serial_port&);
bool cts(asio::serial_port&, asio::error_code&);

bool rts(asio::serial_port&);
bool rts(asio::serial_port&, asio::error_code&);

void rts(asio::serial_port&, bool);
void rts(asio::serial_port&, bool, asio::error_code&);

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
