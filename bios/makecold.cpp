/*
 * Copyright (c) 2023 Dimitry Ishenko <dimitry.ishenko (at) (gee) mail (dot) com>
 * Copyright (c) 2020 Digi International Inc.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "file.hpp"
#include "message.hpp"

#include <asio.hpp>
#include <exception>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

constexpr std::size_t max_size = 32768;

/*
 *	These are the first few bytes of the coldload sequence.
 *	They are used to set up some of the configuration registers.
 */
constexpr char prologue[] {
    "\x80\x00\x08"  // GCSR: no periodic int, main osc no div
    "\x80\x10\x00"  // MMIDR: normal operation
    "\x80\x14\x45"  // MB0CR: 2 ws, /OE1, /WE1, /CS1 active
    "\x80\x15\x45"  // MB1CR: 2 ws, /OE1, /WE1, /CS1 active
    "\x80\x16\x40"  // MB2CR: 2 ws, /OE0, /WE0, /CS0 active
    "\x80\x17\x40"  // MB3CR: 2 ws, /OE0, /WE0, /CS0 active
    "\x80\x13\xc6"  // SEGSIZE
    "\x80\x11\x74"  // STACKSEG
    "\x80\x12\x3a"  // DATASEG
};
constexpr char epilogue[] = "\x80\x24\x80";

int main(int argc, char* argv[])
try
{
    if (argc != 3) {
        auto name = fs::path{argv[0]}.filename().string();
        std::cerr << "Usage: " << name << " <input> <output>" << std::endl;
        return 1;
    };

    auto path_in = argv[1], path_out = argv[2];
    message("input=", path_in, " output=", path_out, "\n");

    asio::io_context ctx;

    payload data_in = read_file(ctx, path_in, max_size);
    if (fs::file_size(path_in) > data_in.size()) {
        message("WARNING: stopped after reading ", data_in.size(), " bytes from input\n");
    }

    auto file_out = open_file(ctx, path_out, flags::write_only | flags::create | flags::truncate);

    payload data_out(data_in.size() * 3);
    do_("Converting input data", [&]{
        std::size_t n = 0;
        for (auto in = data_in.begin(), out = data_out.begin(); in != data_in.end(); ++in, ++out, ++n) {
            *out = n >> 8; *++out = n; *++out = *in;
        }
    });

    do_("Writing start sequence", [&]{ asio::write(file_out, asio::buffer(prologue, size(prologue))); });
    do_("Writing data",           [&]{ asio::write(file_out, asio::buffer(data_out)); });
    do_("Writing end sequence",   [&]{ asio::write(file_out, asio::buffer(epilogue, size(epilogue))); });

    message("Wrote ", file_out.size(), " bytes to output\n");
    return 0;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 2;
}
