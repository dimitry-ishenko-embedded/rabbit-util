////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "serial.hpp"

#include <asio.hpp>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context&, const std::string& name);

using payload = std::vector<char>;
payload read_file(asio::io_context&, const std::string& path);

void reset_target(asio::serial_port&);
void detect_target(asio::serial_port&);

////////////////////////////////////////////////////////////////////////////////
#endif
