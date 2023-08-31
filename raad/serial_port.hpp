////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SERIAL_PORT_HPP
#define SERIAL_PORT_HPP

#include <asio.hpp>

////////////////////////////////////////////////////////////////////////////////
enum baud_rate { };
constexpr baud_rate operator"" _baud(unsigned long long n) noexcept { return static_cast<baud_rate>(n); }

enum bits { };
constexpr bits operator"" _bits(unsigned long long n) noexcept { return static_cast<bits>(n); }

using flow_control = asio::serial_port::flow_control::type;
using parity = asio::serial_port::parity::type;
using stop_bits = asio::serial_port::stop_bits::type;
using char_size = bits;

constexpr bool lo = false;
constexpr bool hi =true;

class serial_port : public asio::serial_port
{
public:
    using asio::serial_port::serial_port;

    void set(::baud_rate);
    void set(::baud_rate, asio::error_code&);

    void set(::flow_control);
    void set(::flow_control, asio::error_code&);

    void set(::parity);
    void set(::parity, asio::error_code&);

    void set(::stop_bits);
    void set(::stop_bits, asio::error_code&);

    void set(::char_size);
    void set(::char_size, asio::error_code&);

    ////////////////////
    void cts(bool);
    void cts(bool, asio::error_code&);

    bool dcd() const;
    bool dcd(asio::error_code&) const;

    bool dsr() const;
    bool dsr(asio::error_code&) const;

    void dtr(bool);
    void dtr(bool, asio::error_code&);

    bool dtr() const;
    bool dtr(asio::error_code&) const;

    bool ri() const;
    bool ri(asio::error_code&) const;

    void rts(bool);
    void rts(bool, asio::error_code&);

    bool rts() const;
    bool rts(asio::error_code&) const;

private:
    void set_bit(int bit, bool, asio::error_code&);
    bool get_bit(int bit, asio::error_code&) const;
};

////////////////////////////////////////////////////////////////////////////////
#endif
