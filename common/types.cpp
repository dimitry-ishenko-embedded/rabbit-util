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
    os << "0x" << std::hex << n;
    return std::move(os).str();
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

// https://datatracker.ietf.org/doc/html/rfc1145
word fletcher8(word init, const byte* data, size_t size)
{
    // NB: Rabbit ordering
    word a = init >> 8, b = init & 0xff;
    for (auto end = data + size; data != end; ++data)
    {
        a += *data; a = (a & 0xff) + (a >> 8);
        b += a; b = (b & 0xff) + (b >> 8);
    }
    return b |= (a << 8);
}
