////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "file.hpp"

#include <asio.hpp>
#include <string>

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context&, const std::string& name);

void reset_target(asio::serial_port&);
void detect_target(asio::serial_port&);

void send_coldload(asio::serial_port&, const payload&);
void send_pilot(asio::serial_port&, const payload&);

////////////////////////////////////////////////////////////////////////////////
#endif
