////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdint> // std::size_t
#include <iostream>
#include <tuple>

inline void message(auto&&... args) { (std::cout << ... << std::forward<decltype (args)>(args)) << std::flush; }
inline void doing(auto&&... args) { message(std::forward<decltype (args)>(args)..., "... "); }

inline void done() { message("OK\n"); }
inline void fail() { message("FAILED\n"); }

namespace detail
{

template<typename... Args, std::size_t... Msg>
void do_(std::tuple<Args...> args, std::index_sequence<Msg...>)
try
{
    doing(std::get<Msg>(args)...);
    std::get<sizeof...(Args) - 1>(args)(); // call function
    done();
}
catch(...) { fail(); throw; }

}

void do_(auto&&... args)
{
    auto msg = std::make_index_sequence<sizeof...(args) - 1>{ };
    detail::do_(std::forward_as_tuple(std::forward<decltype (args)>(args)...), msg);
}

////////////////////////////////////////////////////////////////////////////////
#endif
