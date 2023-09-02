////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "rabbit.hpp"

#include <cmath> // std::round
#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
namespace
{

template<typename... Args>
inline void doing(Args&&... args) { (std::cout << ... << std::forward<Args>(args)) << "... " << std::flush; }

inline void done() { std::cout << "OK" << std::endl; }
inline void fail() { std::cout << "FAILED" << std::endl; }

inline auto to_human(unsigned n)
{
    std::ostringstream os;

    if (n < 1000) os << n << " bytes";
    else if (n < 999950) os << std::round(n / 100.) / 10. << " KB";
    else os << std::round(n / 100000.) / 10. << " MB";

    return std::move(os).str();
}

}

////////////////////////////////////////////////////////////////////////////////
asio::serial_port open_serial(asio::io_context& ctx, const std::string& name)
try
{
    doing("Opening serial port ", name);
    asio::serial_port serial{ctx, name};

    done();
    return std::move(serial);
}
catch(...) { fail(); throw; }

payload read_file(asio::io_context& ctx, const std::string& path)
try
{
    doing("Opening file ", path);
    asio::stream_file file{ctx, path, asio::stream_file::read_only};
    done();

    doing("Reading data");
    doing(to_human(file.size()));

    payload data;
    asio::read(file, asio::dynamic_buffer(data, file.size()));
    done();

    return data;
}
catch(...) { fail(); throw; }
