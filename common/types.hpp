////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef TYPES_HPP
#define TYPES_HPP

#include <chrono>
#include <cstdint>
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

std::string to_human(unsigned);
std::string to_hex(unsigned);
std::string to_hex(const byte* data, size_t size);
inline std::string to_hex(const payload& p) { return to_hex(p.data(), p.size()); }

////////////////////////////////////////////////////////////////////////////////
byte checksum(const byte* data, size_t size);
word fletcher16(word init, const byte* data, size_t size);
inline auto fletcher16(const byte* data, size_t size) { return fletcher16(0, data, size); }

////////////////////////////////////////////////////////////////////////////////
#endif
