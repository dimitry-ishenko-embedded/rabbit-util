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
constexpr dword max_baud_rate = 460800;

// extracted from:
// https://github.com/digidotcom/DCRabbit_9/tree/master/Lib/BIOSLIB/tc_defs.lib
// https://github.com/digidotcom/DCRabbit_9/tree/master/Lib/BIOSLIB/IDBLOCK.LIB
// https://github.com/digidotcom/DCRabbit_9/tree/master/TCData.ini
// https://github.com/digidotcom/DCRabbit_9/tree/master/flash.ini

enum : byte
{
    TC_VERSION              = 0x02,
    TC_TYPE_SYSTEM          = 0x00,
    TC_FRAMING_START        = 0x7e,
    TC_FRAMING_ESC          = 0x7d,

    TC_SYSTEM_READ          = 0x02,
    TC_SYSTEM_WRITE         = 0x03,
    TC_SYSTEM_INFOPROBE     = 0x04,
    TC_SYSTEM_STARTBIOS     = 0x05,
    TC_SYSTEM_SETBAUDRATE   = 0x06,
    TC_SYSTEM_RELOCATE      = 0x08,
    TC_SYSTEM_ERASEFLASH    = 0x09,
    TC_SYSTEM_FLASHDATA     = 0x0a,

    TC_SUBTYPE_MASK         = 0x3f,
    TC_NAK                  = 0x40,
    TC_ACK                  = 0x80,

    TC_SYSWRITE_PHYSICAL    = 0x00,

    TC_STARTBIOS_RAM        = 0x01,
    TC_STARTBIOS_FLASH      = 0x02,
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
    dword _1;
    word flash_id;
    byte _2;
    byte div_19200;
    dword cpu_id;
    struct
    {
        word _1;
        word prod_id;
        byte _2[159];
    }
    id_block;
};

struct flash_data
{
    const char* name;
    struct
    {
        word sec_size;
        word num_sec;
        word flash_size;
        word write_mode;
    }
    param;
};

constexpr size_t write_size = 0x80;

struct write_data
{
    byte type;
    word data_size;
    dword address;
    byte data[write_size];
};
#pragma pack(pop)

const std::map<word, const char*> cpu_info
{
    { 0x000, "Rabbit 2000 rev. IQ2T" },
    { 0x001, "Rabbit 2000 rev. IQ3T" },
    { 0x002, "Rabbit 2000 rev. IQ4T" },
    { 0x003, "Rabbit 2000 rev. IQ5T/UQ5T" },
    { 0x100, "Rabbit 3000 rev. IL1T/IZ1T" },
    { 0x101, "Rabbit 3000 rev. IL2T/IZ2T/UL2T/UZ2T" },
    { 0x200, "Rabbit 4000" },
};

const std::map<word, const char*> board_info
{
    { 0x0101, "BL1800  29MHz, 128K SRAM, 256K Flash" },
    { 0x0100, "BL1810  14MHz, 128K SRAM, 128K Flash" },
    { 0x0102, "BL1820  14MHz, 128K SRAM, 128K Flash" },
    { 0x0800, "BL2000  22MHz, 128K SRAM, 256K Flash" },
    { 0x0806, "BL2000  22MHz, 512K SRAM, 512K Flash" },
    { 0x0801, "BL2010  22MHz, 128K SRAM, 256K Flash" },
    { 0x0802, "BL2020  22MHz, 128K SRAM, 256K Flash" },
    { 0x0803, "BL2030  22MHz, 128K SRAM, 256K Flash" },
    { 0x0804, "BL2040  22MHz, 128K SRAM, 256K Flash" },
    { 0x0B00, "BL2100  22MHz, 128K SRAM, 256K Flash" },
    { 0x0B06, "BL2101  22MHz, 128K SRAM, 256K Flash" },
    { 0x0B04, "BL2105  22MHz, 512K SRAM, 512K Flash" },
    { 0x0B01, "BL2110  22MHz, 128K SRAM, 256K Flash" },
    { 0x0B08, "BL2111  22MHz, 512K SRAM, 512K Flash" },
    { 0x0B05, "BL2115  22MHz, 512K SRAM, 512K Flash" },
    { 0x0B02, "BL2120  22MHz, 128K SRAM, 256K Flash" },
    { 0x0B07, "BL2121  22MHz, 128K SRAM, 256K Flash" },
    { 0x0B03, "BL2130  22MHz, 128K SRAM, 256K Flash" },
    { 0x1500, "BL2500  29MHz, 128K SRAM, 256K Flash" },
    { 0x1502, "BL2500  29MHz, 512K SRAM, 512K Flash" },
    { 0x1504, "BL2500  44MHz, 256K+512K SRAM, 512K Flash" },
    { 0x1501, "BL2510  29MHz, 128K SRAM, 256K Flash" },
    { 0x1503, "BL2510  29MHz, 512K SRAM, 512K Flash" },
    { 0x1700, "BL2600  44MHz, 256K+512K SRAM, 512K Flash" },
    { 0x1701, "BL2600  29MHz, 2x256K Flash, 512K SRAM" },
    { 0x1704, "BL2600  29MHz, 256K Flash, 128K SRAM" },
    { 0x170A, "BL2600  44MHz, 512 FSRAM, 16M NAND Flash" },
    { 0x170B, "BL2600  44MHz, 512 FSRAM"             },
    { 0x170C, "BL2600  44MHz, 512 FSRAM, 16M NAND Flash" },
    { 0x170D, "BL2600  44MHz, 512 FSRAM"             },
    { 0x170F, "BL2600  44MHz, 512 FSRAM, 32M NAND Flash" },
    { 0x1702, "BL2610  29MHz, 512K SRAM, 2x256K Flash" },
    { 0x1705, "BL2610  29MHz, 128K SRAM, 256K Flash" },
    { 0x0601, "EG2100  22MHz, 128K SRAM, 512K Flash" },
    { 0x0603, "EG2110  22MHz, 128K SRAM, 512K Flash" },
    { 0x1200, "LP3500   7MHz, 512K SRAM, 512K Flash" },
    { 0x1201, "LP3510   7MHz, 128K SRAM, 256K Flash" },
    { 0x0300, "OP6600  18MHz, 128K SRAM, 256K Flash" },
    { 0x0302, "OP6700  18MHz, 128K SRAM, 512K Flash" },
    { 0x0D00, "OP6800  22MHz, 128K SRAM, 256K Flash" },
    { 0x0D01, "OP6810  22MHz, 128K SRAM, 256K Flash" },
    { 0x1100, "OP7200  22MHz, 128K SRAM, 256K Flash" },
    { 0x1102, "OP7200  22MHz, 512K SRAM, 512K Flash" },
    { 0x1101, "OP7210  22MHz, 128K SRAM, 256K Flash" },
    { 0x1103, "OP7210  22MHz, 512K SRAM, 512K Flash" },
    { 0x2300, "PowerCore FLEX Board Series"          },
    { 0x2301, "PowerCore FLEX Board Series"          },
    { 0x2400, "RabbitFLEX SBC40 Series"              },
    { 0x0202, "RCM2000 25MHz, 512K SRAM, 256K Flash" },
    { 0x0201, "RCM2010 25MHz, 128K SRAM, 256K Flash" },
    { 0x0200, "RCM2020 18MHz, 128K SRAM, 256K Flash" },
    { 0x0700, "RCM2100 22MHz, 512K SRAM, 512K Flash" },
    { 0x0701, "RCM2110 22MHz, 128K SRAM, 256K Flash" },
    { 0x0702, "RCM2120 22MHz, 512K SRAM, 512K Flash" },
    { 0x0703, "RCM2130 22MHz, 128K SRAM, 256K Flash" },
    { 0x0900, "RCM2200 22MHz, 128K SRAM, 256K Flash" },
    { 0x0E00, "RCM2250 22MHz, 512K SRAM, 512K Flash" },
    { 0x0901, "RCM2260 22MHz, 512K SRAM, 512K Flash" },
    { 0x0A00, "RCM2300 22MHz, 128K SRAM, 256K Flash" },
    { 0x0F00, "RCM3000 29MHz, 512K SRAM, 512K Flash" },
    { 0x0F01, "RCM3010 29MHz, 128K SRAM, 256K Flash" },
    { 0x1000, "RCM3100 29MHz, 512K SRAM, 512K Flash" },
    { 0x1001, "RCM3110 29MHz, 128K SRAM, 256K Flash" },
    { 0x1300, "RCM3200 44MHz, 256K+512K SRAM, 512K Flash" },
    { 0x2D20, "RCM3209 44MHz, 256K+512K SRAM, 512K Flash" },
    { 0x1301, "RCM3210 29MHz, 128K SRAM, 256K Flash" },
    { 0x1302, "RCM3220 44MHz, 256K+512K SRAM, 512K Flash" },
    { 0x2D21, "RCM3229 44MHz, 256K+512K SRAM, 512K Flash" },
    { 0x1400, "RCM3300 44MHz, 512K+512K SRAM, 512K Flash, 8M Serial Flash" },
    { 0x1408, "RCM3305 44MHz, 512K+512K SRAM, 512K Flash, 8M Serial Flash" },
    { 0x2D30, "RCM3309 44MHz, 512K+512K SRAM, 512K Flash, 8M Serial Flash" },
    { 0x1402, "RCM3310 44MHz, 512K+512K SRAM, 512K Flash, 4M Serial Flash" },
    { 0x1409, "RCM3315 44MHz, 512K+512K SRAM, 512K Flash, 4M Serial Flash" },
    { 0x2D31, "RCM3319 44MHz, 512K+512K SRAM, 512K Flash, 4M Serial Flash" },
    { 0x1403, "RCM3360 44MHz, 512K+512K SRAM, 512K Flash, 16M NAND Flash" },
    { 0x1405, "RCM3360 44MHz, 512K+512K SRAM, 512K Flash, 16M NAND Flash" },
    { 0x1406, "RCM3365 44MHz, 512K+512K SRAM, 512K Flash, 16M NAND Flash" },
    { 0x1404, "RCM3370 44MHz, 512K+512K SRAM, 512K Flash" },
    { 0x1407, "RCM3375 44MHz, 512K+512K SRAM, 512K Flash" },
    { 0x1600, "RCM3400 29MHz, 512K SRAM, 512K Flash" },
    { 0x1610, "RCM3410 29MHz, 256K SRAM, 256K Flash" },
    { 0x1E00, "RCM3600 22MHz, 512K SRAM, 512K Flash" },
    { 0x1E01, "RCM3610 22MHz, 128K SRAM, 256K Flash" },
    { 0x1F00, "RCM3700 22MHz, 512K SRAM, 512K Flash, 1M serial flash" },
    { 0x1F01, "RCM3710 22MHz, 128K SRAM, 256K Flash, 1M serial flash" },
    { 0x1F02, "RCM3720 22MHz, 256K SRAM, 512K Flash, 1M serial flash" },
    { 0x1F04, "RCM3750 22MHz, 512K SRAM, 512K Flash, 1M serial flash" },
    { 0x1F05, "RCM3760 22MHz, 512K SRAM, 512K Flash, 1M serial flash, 3.3V" },
    { 0x2D00, "RCM3900 44MHz, 512K+512K SRAM, 512K Flash, 32M NAND Flash" },
    { 0x2D01, "RCM3910 44MHz, 512K+512K SRAM, 512K Flash" },
    { 0x0502, "SR9100  25MHz, 128K SRAM, 512K Flash" },
    { 0x0C00, "SR9150  22MHz, 128K SRAM, 512K Flash" },
    { 0x0C01, "SR9160  22MHz, 128K SRAM, 512K Flash" },
};

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

