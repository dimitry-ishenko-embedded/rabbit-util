////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef RAAD_HPP
#define RAAD_HPP

#include "types.hpp"
#include <asio.hpp>

struct params
{
    bool run = false;
    bool run_in_ram = false;
    bool slow = false;
    bool use_cts = false;
    bool use_rts = false;
};

void reset_target(asio::serial_port&, const params&);
void detect_target(asio::serial_port&, const params&);

void send_coldload(asio::serial_port&, const payload&, const params&);
void send_pilot(asio::serial_port&, const payload&);

void send_program(asio::serial_port&, const payload&, const params&);

////////////////////////////////////////////////////////////////////////////////
#endif
