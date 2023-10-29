////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "file.hpp"
#include "message.hpp"
#include "types.hpp"

#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////
void stream_file::open(const std::string& path, flags flags)
{
    int desc = ::open(path.data(), static_cast<int>(flags), 0644);
    if (desc < 0) asio::detail::throw_error(
        asio::error_code{errno, std::system_category()}, "stream_file::open"
    );
    assign(desc);
}

size_t stream_file::size() const
{
    struct stat st;
    int ret = fstat(const_cast<stream_file*>(this)->native_handle(), &st);
    if (ret == -1) asio::detail::throw_error(
        asio::error_code{errno, std::system_category()}, "stream_file::size"
    );
    return st.st_size;
}

////////////////////////////////////////////////////////////////////////////////
stream_file open_file(asio::io_context& ctx, const std::string& path, flags flags)
{
    stream_file file{ctx};
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
