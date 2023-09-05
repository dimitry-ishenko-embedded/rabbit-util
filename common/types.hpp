////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef TYPES_HPP
#define TYPES_HPP

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;
using namespace std::this_thread;

////////////////////////////////////////////////////////////////////////////////
using byte = std::uint8_t;
using word = std::uint16_t;
using dword = std::uint32_t;
using std::size_t;

using payload = std::vector<byte>;

enum { lo = false, hi = true };

////////////////////////////////////////////////////////////////////////////////
auto addressof(auto& val) { return reinterpret_cast<byte*>(&val); }

template<size_t N>
constexpr auto size(const byte (&)[N]) { return N - 1; }

inline auto to_human(unsigned n)
{
    std::ostringstream os;

    if (n < 1000) os << n << " bytes";
    else if (n < 999950) os << std::round(n / 100.) / 10. << " KB";
    else os << std::round(n / 100000.) / 10. << " MB";

    return std::move(os).str();
}

inline auto to_hex(int val)
{
    std::ostringstream os;
    os << std::hex << val;
    return "0x" + std::move(os).str();
}

////////////////////////////////////////////////////////////////////////////////
inline auto checksum(const byte* data, size_t size)
{
    byte check = 0;
    for (auto end = data + size; data != end; ++data) check += *data;
    return check;
}

// https://en.wikipedia.org/wiki/Fletcher's_checksum#Implementation
inline auto fletcher16(word init, const byte* data, size_t size)
{
    // NB: Rabbit ordering
    word a = init >> 8, b = init & 0xff;
    for (auto end = data + size; data != end; ++data) { a = (a + *data) % 255; b = (b + a) % 255; }
    return b |= (a << 8);
}
inline auto fletcher16(const byte* data, size_t size) { return fletcher16(0, data, size); }

////////////////////////////////////////////////////////////////////////////////
#endif
