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

using flags = asio::stream_file::flags;

asio::stream_file open_file(asio::io_context&, const std::string& path, flags);
payload read_file(asio::io_context&, const std::string& path, size_t max_size = 0);

////////////////////////////////////////////////////////////////////////////////
#endif
