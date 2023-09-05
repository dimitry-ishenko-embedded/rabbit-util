////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "file.hpp"
#include "message.hpp"
#include "types.hpp"

////////////////////////////////////////////////////////////////////////////////
asio::stream_file open_file(asio::io_context& ctx, const std::string& path, flags flags)
{
    asio::stream_file file{ctx};
    do_("Opening file ", path, [&]{ file.open(path, flags); });
    return file;
}

payload read_file(asio::io_context& ctx, const std::string& path, size_t max_size)
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
