/*
 * Copyright (c) 2023 Dimitry Ishenko <dimitry.ishenko (at) (gee) mail (dot) com>
 * Copyright (c) 2020 Digi International Inc.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

constexpr std::size_t max_size = 32768;

/*
 *	These are the first few bytes of the coldload sequence.
 *	They are used to set up some of the configuration registers.
 */
constexpr char prologue[]
{
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
    if (argc != 3)
    {
        auto name = fs::path{argv[0]}.filename().string();
        std::cerr << "Usage: " << name << " <input> <output>" << std::endl;
        return 1;
    };

    auto path_in = fs::path{argv[1]}, path_out = fs::path{argv[2]};
    std::cout << "input=" << path_in << " output=" << path_out << std::endl;

    std::vector<char> data_in, data_out;

    if (std::ifstream file_in{path_in}; file_in)
    {
        auto size = fs::file_size(path_in);
        data_in.resize( std::min(size, max_size) );

        file_in.read(data_in.data(), data_in.size());
        if (!file_in) throw std::runtime_error{"Error reading from input"};

        if (size > max_size) std::cout << "Warning: stopped after reading " << max_size << " bytes from input" << std::endl;
    }
    else throw std::runtime_error{"Error opening input file"};

    if (std::ofstream file_out{path_out}; file_out)
    {
        file_out.write(prologue, sizeof(prologue) - 1);
        if (!file_out) throw std::runtime_error{"Error writing start sequence"};

        data_out.resize(data_in.size() * 3);

        std::size_t n = 0;
        for (auto in = data_in.begin(), out = data_out.begin(); in != data_in.end(); ++in, ++out, ++n)
        {
            *out = n >> 8; *++out = n; *++out = *in;
        }

        file_out.write(data_out.data(), data_out.size());
        if (!file_out) throw std::runtime_error{"Error writing to output"};

        file_out.write(epilogue, sizeof(epilogue) - 1);
        if (!file_out) throw std::runtime_error{"Error writing end sequence"};
    }
    else throw std::runtime_error{"Error opening output file"};

    std::cout << "Wrote " << fs::file_size(path_out) << " bytes to output" << std::endl;
    return 0;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 2;
}
