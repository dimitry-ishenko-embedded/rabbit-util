////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2023 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "types.hpp"
#include <map>

////////////////////////////////////////////////////////////////////////////////
constexpr dword min_baud_rate = 57600;
constexpr dword max_baud_rate = 115200;

// extracted from:
// https://github.com/digidotcom/DCRabbit_9/tree/master/Lib/BIOSLIB/tc_defs.lib
// https://github.com/digidotcom/DCRabbit_9/tree/master/Lib/BIOSLIB/IDBLOCK.LIB
// https://github.com/digidotcom/DCRabbit_9/tree/master/flash.ini

enum : byte
{
    TC_VERSION              = 0x02, // the main version number
    TC_TYPE_SYSTEM          = 0x00, // system messages
    TC_FRAMING_START        = 0x7e,
    TC_FRAMING_ESC          = 0x7d,

    TC_SYSTEM_READ          = 0x02, // read memory
    TC_SYSTEM_WRITE         = 0x03, // write memory
    TC_SYSTEM_INFOPROBE     = 0x04, // probe configuration information
    TC_SYSTEM_STARTBIOS     = 0x05, // signal the pilot BIOS to start running the real BIOS
    TC_SYSTEM_SETBAUDRATE   = 0x06, // change the baud rate of the pilot BIOS
    TC_SYSTEM_RELOCATE      = 0x08, // tell the pilot BIOS to relocate itself in the physical address space
    TC_SYSTEM_ERASEFLASH    = 0x09, // tell the pilot BIOS to erase the entire flash
    TC_SYSTEM_FLASHDATA     = 0x0a, // send the flash information to the pilot BIOS

    TC_SUBTYPE_MASK         = 0x3f,
    TC_NAK                  = 0x40,
    TC_ACK                  = 0x80,
};

#pragma pack(push, 1)
struct pilot_head
{
    dword address;
    word size;
    byte check;
};

struct packet_head
{
    byte version;
    byte flags;
    byte type;
    byte subtype;
    word data_size;
    word check;
};

struct info_probe
{
    dword location;     // physical address of ID block
    word  flash_id;     // 16-bit flash identifier
    byte  ram_size;     // 8-bit code for the ram size
    byte  div_19200;    // frequency divider for 19200 baud
    dword cpu_id;       // identifiers about the CPU
    struct
    {
        byte _1[55];
        byte mac_addr[6];
        byte serial_num[24];
        byte product_name[30];
        byte _2[48];
    }
    id_block;
};

struct flash_data
{
    const char* info;
    word sec_size;
    word num_sec;
    word flash_size;
    word write_mode;
};
#pragma pack(pop)

inline const std::map<dword, flash_data> flash_info
{
    { 0x0134, { "AMD AM29F002BB",       128,  7,    64,  0x16 } },
    { 0x0140, { "AMD AM29LV002BT",      128,  7,    64,  0x15 } },
    { 0x016d, { "AMD AM29LV001BB",      128,  10,   32,  0x1f } },
    { 0x0177, { "AMD AM29F004BT",       128,  11,   128, 0x17 } },
    { 0x017b, { "AMD AM29F004BB",       128,  11,   128, 0x18 } },
    { 0x01a4, { "AMD AM29F040B",        128,  8,    128, 0x1a } },
    { 0x01b0, { "AMD AM29F002BT",       128,  7,    64,  0x15 } },
    { 0x01b5, { "AMD AM29LV004BT",      128,  11,   128, 0x17 } },
    { 0x01b6, { "AMD AM29LV004BB",      128,  11,   128, 0x18 } },
    { 0x01c2, { "AMD AM29LV002BB",      128,  7,    64,  0x16 } },
    { 0x01ed, { "AMD AM29LV001BT",      128,  10,   32,  0x13 } },
    { 0x0434, { "Fujitsu MBM29F002BC",  128,  7,    64,  0x16 } },
    { 0x04b0, { "Fujitsu MBM29F002TC",  128,  7,    64,  0x15 } },
    { 0x1f07, { "Atmel AT49F002",       128,  5,    64,  0x12 } },
    { 0x1f08, { "Atmel AT49F002T",      128,  5,    64,  0x11 } },
    { 0x1f35, { "Atmel AT29[LB]V010[A]",128,  1024, 32,  2    } },
    { 0x1fa4, { "Atmel AT29C040",       256,  2048, 128, 2    } },
    { 0x1fba, { "Atmel AT29[LB]V020",   256,  1024, 64,  2    } },
    { 0x1fc4, { "Atmel AT29[LB]V040",   256,  2048, 128, 2    } },
    { 0x1fd5, { "Atmel AT29C010",       128,  1024, 32,  2    } },
    { 0x1fda, { "Atmel AT29C020",       256,  1024, 64,  2    } },
    { 0x2023, { "STM M29W010B",         128,  8,    32,  0x20 } },
    { 0x20e2, { "STM M29F040B",         128,  8,    128, 0x1a } },
    { 0x20e3, { "STM M29W040B",         128,  8,    128, 0x1a } },
    { 0x4001, { "Mosel V29C51001T",     512,  256,  32,  1    } },
    { 0x4002, { "Mosel V29C51002T",     512,  512,  64,  1    } },
    { 0x4003, { "Mosel V29C51004T",     1024, 512,  128, 1    } },
    { 0x4060, { "Mosel V29LC51001",     512,  256,  32,  1    } },
    { 0x4063, { "Mosel V29C31004T",     1024, 512,  128, 1    } },
    { 0x4073, { "Mosel V29C31004B",     1024, 512,  128, 1    } },
    { 0x4082, { "Mosel V29LC51002",     512,  512,  32,  1    } },
    { 0x40a1, { "Mosel V29C51001B",     512,  256,  32,  1    } },
    { 0x40a2, { "Mosel V29C51002B",     512,  512,  64,  1    } },
    { 0x40a3, { "Mosel V29C51004B",     1024, 512,  128, 1    } },
    { 0xad34, { "Hyundai Hy29F002B",    128,  7,    64,  0x16 } },
    { 0xadb0, { "Hynix HY29F002T",      128,  7,    64,  0x15 } },
    { 0xbf07, { "SST SST29EE010",       128,  1024, 32,  2    } },
    { 0xbf08, { "SST SST29[LV]E010",    128,  1024, 32,  2    } },
    { 0xbf10, { "SST SST29EE020",       128,  2048, 64,  2    } },
    { 0xbf12, { "SST SST29[LV]E020",    128,  2048, 64,  2    } },
    { 0xbf13, { "SST SST29SF040",       128,  2048, 128, 4    } },
    { 0xbf14, { "SST SST29VF040",       128,  2048, 128, 4    } },
    { 0xbf20, { "SST SST29SF512",       128,  512,  16,  4    } },
    { 0xbf21, { "SST SST29VF512",       128,  512,  16,  4    } },
    { 0xbf22, { "SST SST29SF010",       128,  1024, 32,  4    } },
    { 0xbf23, { "SST SST29VF010",       128,  1024, 32,  4    } },
    { 0xbf24, { "SST SST29SF020",       128,  2048, 64,  4    } },
    { 0xbf25, { "SST SST29VF020",       128,  2048, 64,  4    } },
    { 0xbf3d, { "SST SST29[LV]E512",    128,  512,  16,  2    } },
    { 0xbf5d, { "SST SST29EE512",       128,  512,  16,  2    } },
    { 0xbfb4, { "SST SST39SF512",       4096, 16,   16,  1    } },
    { 0xbfb5, { "SST SST39SF010",       4096, 32,   32,  1    } },
    { 0xbfb6, { "SST SST39SF020",       4096, 64,   64,  1    } },
    { 0xbfb7, { "SST SST39SF040",       4096, 128,  128, 1    } },
    { 0xbfd4, { "SST SST39[LV]F512",    4096, 16,   16,  1    } },
    { 0xbfd5, { "SST SST39[LV]F010",    4096, 32,   32,  1    } },
    { 0xbfd6, { "SST SST39[LV]F020",    4096, 64,   64,  1    } },
    { 0xbfd7, { "SST SST39[LV]F040",    4096, 128,  128, 1    } },
    { 0xc234, { "Macronix MX29F002B",   128,  7,    64,  0x16 } },
    { 0xc2b0, { "Macronix MX29F002T",   128,  7,    64,  0x15 } },
    { 0xda45, { "Winbond W29C020CT",    128,  2048, 64,  2    } },
    { 0xda46, { "Winbond W29C040",      256,  2048, 128, 2    } },
    { 0xdab5, { "Winbond W39L020",      4096, 64,   64,  3    } },
    { 0xdac1, { "Winbond W29EE011",     128,  1024, 32,  2    } },
};

////////////////////////////////////////////////////////////////////////////////
#endif

