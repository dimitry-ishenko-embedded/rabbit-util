////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "pgm/args.hpp"
#include "rabbit.hpp"

#include <asio.hpp>
#include <exception>
#include <filesystem>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
constexpr auto def_coldload = BIOS_DIR "/coldload.bin";
constexpr auto def_pilot = BIOS_DIR "/pilot.bin";

int main(int argc, char* argv[])
try
{
    const auto name = std::filesystem::path{argv[0]}.filename().string();

    pgm::args args
    {
        { "-p", "name", pgm::req, "Serial port to use for upload."  },
        { "-1", "path",           "Use custom initial loader."      },
        { "-2", "path",           "Use custom secondary loader.\n"  },

        { "-h", "--help",         "Show this help screen and exit." },
        { "-v", "--version",      "Show version and exit."          },

        { "program.bin",          "Path to program to be uploaded." },
    };

    std::exception_ptr ep;
    try { args.parse(argc, argv); }
    catch (...) { ep = std::current_exception(); }

    if (args["--help"])
    {
        std::cout << args.usage(name) << std::endl;
    }
    else if (args["--version"])
    {
        std::cout << name << " version " << VERSION << std::endl;
    }
    else if (ep)
    {
        std::cerr << args.usage(name) << std::endl << std::endl;
        std::rethrow_exception(ep);
    }
    else
    {
        asio::io_context ctx;
        auto serial = open_serial(ctx, args["-p"].value());

        auto coldload = read_file(ctx, args["-1"].value_or(def_coldload));
        auto pilot    = read_file(ctx, args["-2"].value_or(def_pilot));
        auto program  = read_file(ctx, args["program.bin"].value());

        //
    }

    return 0;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
};
