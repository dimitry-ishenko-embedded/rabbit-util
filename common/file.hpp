////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef FILE_HPP
#define FILE_HPP

#include "types.hpp"
#include <asio.hpp>

enum class flags
{
    create      = O_CREAT,
    read_only   = O_RDONLY,
    read_write  = O_RDWR,
    truncate    = O_TRUNC,
    write_only  = O_WRONLY,
};

constexpr inline flags operator|(flags l, flags r) { return static_cast<flags>(static_cast<int>(l) | static_cast<int>(r)); }

struct stream_file : asio::posix::stream_descriptor
{
    stream_file(asio::io_context& ctx) : asio::posix::stream_descriptor{ctx} { }

    void open(const std::string& path, flags);
    size_t size() const;
};

stream_file open_file(asio::io_context&, const std::string& path, flags);
payload read_file(asio::io_context&, const std::string& path, size_t max_size = 0);

////////////////////////////////////////////////////////////////////////////////
#endif
