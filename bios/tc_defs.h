/*
   Copyright (c) 2015 Digi International Inc.

   This Source Code Form is subject to the terms of the Mozilla Public
   License, v. 2.0. If a copy of the MPL was not distributed with this
   file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef TC_DEFS_H
#define TC_DEFS_H

#include "misc_types.h"

// the main version number
#define TC_VERSION 0x02

/*
 * This value is the maximum number of applications (read: callbacks) that are
 * supported. Therefore, the largest 'type' value supported is
 * TC_MAX_APPLICATIONS - 1
 */
#define TC_MAX_APPLICATIONS 8

// standard TYPE (i.e. application) values
#define TC_TYPE_SYSTEM          0x00 // System messages
#define TC_TYPE_DEBUG           0x01 // Debugging messages
#define TC_TYPE_WD              0x02 // Watchdogs
#define TC_TYPE_TCPIP           0x03 // TCP/IP tunneling over TC/XTC
#define TC_TYPE_FS              0x04 // File system
#define TC_TYPE_VAR             0x05 // Web-page variable
#define TC_TYPE_LOG             0x06 // Logging
#define TC_TYPE_SMTP            0x07 // E-Z email

// packet framing characters
#define TC_FRAMING_START        0x7e
#define TC_FRAMING_ESC          0x7d

// return values
#define TC_SUCCESS               0
#define TC_ERROR                -1
#define TC_NOCALLBACK           -2
#define TC_PENDING              -3
#define TC_TIMEOUT              -4
#define TC_TRUNCATED            -5

// values of 'flags' in the callback
#define TC_RECEIVE              0x0001
#define TC_TXDONE               0x0002
#define TC_UNSUPPORTED          0x0004
#define TC_NOBUFFER             0x0008
#define TC_TOOBIG               0x0010
#define TC_RESET                0x0020
#define TC_SYSBUF               0x8000

// standard ACK/NAK values
#define TC_ACK                  0x80
#define TC_NAK                  0x40

// SYS NAK values
#define TC_NAK_NOAPP            0x01
#define TC_NAK_NOBUF            0x02
#define TC_NAK_TOOBIG           0x04

// SYSTEM subtypes, that are handled internally
#define TC_SYSTEM_NAK           0x00 // TC.LIB generated NAK packets
#define TC_SYSTEM_NOOP          0x01 // NOOPs - reflects the packet back, w/ the subtype ORed with TC_ACK
#define TC_SYSTEM_READ          0x02 // Read memory
#define TC_SYSTEM_WRITE         0x03 // Write memory
#define TC_SYSTEM_INFOPROBE     0x04 // probe (out of the PilotBios) various configuration information
#define TC_SYSTEM_STARTBIOS     0x05 // signal the PilotBios to start running the real bios
#define TC_SYSTEM_SETBAUDRATE   0x06 // change the baud rate of the PilotBIOS/BIOS programming connection
#define TC_SYSTEM_SETOPTIONS    0x07 // setsockopt() style options to the PilotBIOS
#define TC_SYSTEM_RELOCATE      0x08 // tell the PilotBIOS to relocate itself in the physical address space
#define TC_SYSTEM_ERASEFLASH    0x09 // tell the PilotBIOS to erase the entire flash
#define TC_SYSTEM_FLASHDATA     0x0a // send the flash information to the pilot BIOS
#define TC_SYSTEM_RABBITSYS     0x0b // probe for presence of RabbitSys
#define TC_SYSTEM_RSSTARTUSER   0x0c // instruct RabbitSys to start user program
#define TC_SYSTEM_SETDEBUGTAG   0x0d
#define TC_SYSTEM_GETDEBUGTAG   0x0e

/*
 * the main header - this will be auto-generated for each packet, based on the
 * information provided in the tc_send() function
 */
typedef struct {
    uint8_t  version;
    uint8_t  flags;
    uint8_t  type;
    uint8_t  subtype;
    uint16_t length;
    uint16_t header_checksum;
} _TC_PacketHeader;

#define TC_HEADER_SIZE sizeof(_TC_PacketHeader)

// the main footer - this will be auto-generated for each packet
typedef struct {
    uint16_t checksum;
} _TC_PacketFooter;

#define TC_FOOTER_SIZE sizeof(_TC_PacketFooter)

// buffer header - space for this needs to be allocated by the user in their buffers
typedef struct {
    faraddr_t next;
    uint16_t  flags;
    uint16_t  length;
    uint8_t   type;
    uint8_t   subtype;
    uint32_t  userdata;
} _TCHeader;

#define TC_HEADER_RESERVE sizeof(_TCHeader)

// offsets for the header
#define TC_HEADER_NEXT          0x00
#define TC_HEADER_FLAGS         TC_HEADER_NEXT    + 0x04
#define TC_HEADER_LENGTH        TC_HEADER_FLAGS   + 0x02
#define TC_HEADER_TYPE          TC_HEADER_LENGTH  + 0x02
#define TC_HEADER_SUBTYPE       TC_HEADER_TYPE    + 0x01
#define TC_HEADER_USERDATA      TC_HEADER_SUBTYPE + 0x01

/*
 * Note: currently the following packet structures are not intended
 * for target processors other than the Rabbit.
 */

// the READ system packet type
typedef struct {
    uint8_t  type;
    uint16_t length;
    union {
        uint32_t physical;      // if(type == TC_SYSREAD_PHYSICAL)
        struct {
            uint16_t offset;    // if(type & TC_SYSREAD_LOGICAL)
            uint16_t xpc;       // if(!(type & TC_SYSREAD_NOXPC))
        } logical;
    } address;
} _TCSystemREAD;

#define TC_SYSREAD_PHYSICAL     0x00
#define TC_SYSREAD_LOGICAL      0x01
// If I&D and (TC_SYSREAD_LOGICAL_DATA & type) check MMIDR for A16, and A19 inversions
#define TC_SYSREAD_LOGICAL_DATA TC_SYSREAD_LOGICAL
// If I&D and (TC_SYSREAD_LOCICAL_CODE & type) check MMIDR for A16, and A19 inversions
#define TC_SYSREAD_LOGICAL_CODE 0x04
#define TC_SYSREAD_NOXPC        0x02 // Flag for logical read to not read xpc

typedef struct {
    uint16_t length;
    uint32_t address;           // the address read from, regardless of request type
//  char*    data;
} _TCSystemREADACK;

typedef struct {
    uint8_t  type;
    uint16_t length;
    union {
        uint32_t physical;      // if(type == TC_SYSWRITE_PHYSICAL)
        struct {
            uint16_t offset;    // if(type & TC_SYSWRITE_LOGICAL)
            uint16_t xpc;       // if(!(type & TC_SYSWRITE_NOXPC))
        } logical;
    } address;
//  char* data;
} _TCSystemWRITE;

#define TC_SYSTEM_WRITE_HEADERSIZE sizeof(_TCSystemWRITE)

#define TC_SYSWRITE_PHYSICAL    0x00
#define TC_SYSWRITE_LOGICAL     0x01
#define TC_SYSWRITE_NOXPC       0x03

typedef struct {
    uint32_t location;          // physical address of ID block
    uint16_t FlashID;           // 16-bit flash identifier
    char     RamSize;           // 8-bit code for the ram size
    char     div19200;          // frequency divider for 19200 baud
    char     CpuID[4];          // identifiers about the CPU
    SysIDBlockType IDBlock;     // the IDBlock struct
} _TCSystemInfoProbe;

typedef struct {
    char start_mode;            // how the real bios should be started
} _TCSystemSTARTBIOS;

#define TC_STARTBIOS_RAM        0x01 // RAM program - just jump to 0x0000
#define TC_STARTBIOS_FLASH      0x02 // FLAHS program - remap quad-0 to flash and jump to 0x0000

typedef struct {
    uint16_t version;           // RabbitSys version
    uint32_t timestamp;         // RabbitSys build timestamp
} _TCSystemRSVersionProbe;

typedef struct {
    long baud_rate;             // the BAUD rate the programming link will be set to
} _TCSystemSETBAUDRATE;

/*
 * Note: If the specified baudrate is acceptable, SETBAUDRATE|ACK should be
 * returned, and only AFTER that packet has been sent should the baudrate
 * be changed! If the value is not acceptable, returning SETBAUDRATE|NAK is
 * important.
 */

typedef struct {
    uint16_t name;              // the option to change
    uint16_t val;               // the new value for this option
} _TCSystemOptionRecord;

typedef struct {
    uint16_t num_records;       // number of options in this packet
//  _TCSystemOptionRecord records[<num_records>];
} _TCSystemSETOPTIONS;

typedef struct {
    long new_locaton;           // the new location of the pilot bios (physical address)
} _TCSystemRELOCATE;

#endif
