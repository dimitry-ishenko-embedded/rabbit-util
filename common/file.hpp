////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef FILE_HPP
#define FILE_HPP

#include "message.hpp"

#include <asio.hpp>
#include <cmath>
#include <cstdint> // std::size_t
#include <sstream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
using payload = std::vector<char>;

template<std::size_t N>
constexpr auto size(const char (&)[N]) { return N - 1; }

inline auto to_human(unsigned n)
{
    std::ostringstream os;

    if (n < 1000) os << n << " bytes";
    else if (n < 999950) os << std::round(n / 100.) / 10. << " KB";
    else os << std::round(n / 100000.) / 10. << " MB";

    return std::move(os).str();
}

////////////////////////////////////////////////////////////////////////////////
using flags = asio::stream_file::flags;

inline auto open_file(asio::io_context& ctx, const std::string& path, flags flags)
{
    asio::stream_file file{ctx};
    do_("Opening file ", path, [&]{ file.open(path, flags); });
    return file;
}

inline auto read_file(asio::io_context& ctx, const std::string& path, std::size_t max_size = 0)
{
    auto file = open_file(ctx, path, flags::read_only);

    payload data;
    do_("Reading data", [&]{
        if (max_size == 0) max_size = file.size();
        else if (max_size > file.size()) max_size = file.size();

        doing(to_human(max_size));
        asio::read(file, asio::dynamic_buffer(data, max_size));
    });

    return data;
}

////////////////////////////////////////////////////////////////////////////////
#endif
