/*
 * Copyright (c) 2023 Dimitry Ishenko <dimitry.ishenko (at) (gee) mail (dot) com>
 * Copyright (c) 2020 Digi International Inc.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <fstream>
#include <iostream>
#include <stdexcept>

/*
 *	These are the first few bytes of the coldload sequence.
 *	They are used to set up some of the configuration registers.
 */
constexpr char start_seq[] =
    "\x80\x00\x08"  // GCSR: no periodic int, main osc no div
    "\x80\x10\x00"  // MMIDR: normal operation
    "\x80\x14\x45"	// MB0CR: 2 ws, /OE1, /WE1, /CS1 active
    "\x80\x15\x45"	// MB1CR: 2 ws, /OE1, /WE1, /CS1 active
    "\x80\x16\x40"  // MB2CR: 2 ws, /OE0, /WE0, /CS0 active
    "\x80\x17\x40"  // MB3CR: 2 ws, /OE0, /WE0, /CS0 active
    "\x80\x13\xc6"  // SEGSIZE
    "\x80\x11\x74"  // STACKSEG
    "\x80\x12\x3a"; // DATASEG

int main(int argc, char* argv[])
try
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input> <output>" << std::endl;
        return 1;
    };

    std::cout << "input=" << argv[1] << " output=" << argv[2] << std::endl;

    std::ifstream in{argv[1]};
    if (!in) throw std::runtime_error{"Error opening input file"};

    std::ofstream out{argv[2]};
    if (!out) throw std::runtime_error{"Error opening output file"};

    out.write(start_seq, sizeof(start_seq) - 1);
    if (!out) throw std::runtime_error{"Error writing start sequence"};

    char seq[3];

    int i;
    for (i = 0; i < 32768; ++i)
    {
        auto ch = in.get();
        if (in.eof()) break;
        if (!in) throw std::runtime_error{"Error reading from input"};

        seq[0] = i >> 8; seq[1] = i; seq[2] = ch;
        out.write(seq, sizeof(seq));
        if (!out) throw std::runtime_error{"Error writing to output"};
    }

    seq[0] = 0x80; seq[1] = 0x24; seq[2] = 0x80;
    out.write(seq, sizeof(seq));
    if (!out) throw std::runtime_error{"Error writing 0x80 0x24 0x80 to output"};

    if (!in.eof()) std::cout << "Warning: stopped after reading " << i << " bytes from input" << std::endl;
    std::cout << "Wrote " << i * 3 + sizeof(start_seq) - 1 << " bytes to output" << std::endl;

    return 0;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 2;
}
