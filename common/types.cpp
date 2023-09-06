////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "types.hpp"

#include <cmath>
#include <iomanip>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
std::string to_human(unsigned n)
{
    std::ostringstream os;

    if (n < 1000) os << n << " bytes";
    else if (n < 999950) os << std::round(n / 100.) / 10. << " KB";
    else os << std::round(n / 100000.) / 10. << " MB";

    return std::move(os).str();
}

std::string to_hex(unsigned n)
{
    std::ostringstream os;
    os << std::hex << n;
    return "0x" + std::move(os).str();
}

std::string to_hex(const byte* data, size_t size)
{
    std::string s;
    for (auto end = data + size; data != end; ++data) s += to_hex(*data) + ' ';
    return s;
}

////////////////////////////////////////////////////////////////////////////////
byte checksum(const byte* data, size_t size)
{
    byte check = 0;
    for (auto end = data + size; data != end; ++data) check += *data;
    return check;
}

// https://en.wikipedia.org/wiki/Fletcher's_checksum#Implementation
word fletcher16(word init, const byte* data, size_t size)
{
    // NB: Rabbit ordering
    word a = init >> 8, b = init & 0xff;
    for (auto end = data + size; data != end; ++data) { a = (a + *data) % 255; b = (b + a) % 255; }
    return b |= (a << 8);
}
