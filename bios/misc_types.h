/*
 * Copyright (c) 2015 Digi International Inc.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MISC_TYPES_H
#define MISC_TYPES_H

#include <stdint.h>

typedef struct _SysIDBlockType2 {
    uint8_t  flashMBC;          // +0   memory bank configurations
    uint8_t  flash2MBC;         // +1
    uint8_t  ramMBC;            // +2
    uint32_t devSpecLoc;        // +3   count of additional memory devices immediately preceding this block
    uint32_t macrosLoc;         // +7   start of the macro table for additional board configuration options
    uint32_t driversLoc;        // +11  offset to preloaded drivers start from ID block start (positive is below ID block)
    uint32_t ioDescLoc;         // +15  offset to I/O descriptions start from ID block start (positive is below ID block)
    uint32_t ioPermLoc;         // +19  offset to User mode I/O permissions start from ID block start (positive is below ID block)
    uint32_t persBlockLoc;      // +23  offset to persistent storage block area start from ID block start (positive is below ID block)
    uint16_t userBlockSiz2;     // +27  size of v5 "new style" mirrored User block image
    uint16_t idBlockCRC2;       // +29  CRC of SysIDBlockType2 type with idBlockCRC2 member reset to zero and base CRC value of SysIDBlock.idBlockCRC
} SysIDBlockType2;

#define _SysIDBlockType2_size   31
_Static_assert(sizeof(SysIDBlockType2) == _SysIDBlockType2_size, "Incorrect _SysIDBlockType2 size");

typedef struct _SysIDBlockType {
    uint16_t tableVersion;      // +0   version number for this table layout
    uint16_t productID;         // +2   Z-World part #
    uint16_t vendorID;          // +4   1 = Z-World
    uint8_t  timestamp[7];      // +6   yy/m/d h:m:s
    uint32_t flashID;           // +13  Z-World part # dev on CS0 OE0 (normally primary flash)
    uint16_t flashType;         // +17  write method
    uint16_t flashSize;         // +19  in 1000h pages
    uint16_t sectorSize;        // +21  size of flash sector in bytes
    uint16_t numSectors;        // +23  number of sectors
    uint16_t flashSpeed;        // +25  in nanoseconds
    uint32_t flash2ID;          // +27  Z-World part #, 2nd flash
    uint16_t flash2Type;        // +31  write method, 2nd flash
    uint16_t flash2Size;        // +33  in 1000h pages, 2nd flash
    uint16_t sector2Size;       // +35  size of 2nd flash's sectors in bytes
    uint16_t num2Sectors;       // +37  number of sectors
    uint16_t flash2Speed;       // +39  in nanoseconds, 2nd flash
    uint32_t ramID;             // +41  Z-World part #
    uint16_t ramSize;           // +45  in 1000h pages
    uint16_t ramSpeed;          // +47  in nanoseconds
    uint16_t cpuID;             // +49  CPU type identification
    uint32_t crystalFreq;       // +51  in Hertz
    uint8_t  macAddr[6];        // +55  MAC address
    uint8_t  serialNumber[24];  // +61  device serial number
    uint8_t  productName[30];   // +85  null-terminated string

    SysIDBlockType2 idBlock2;   // +115 idblock
    uint8_t  reserved[1];       // +146 allow future expansion (eg, see addition of v5's idblock2 member structure, above)
    uint32_t idBlockSize;       // +147 size of versions 1,2,3,4 ID block
    uint16_t userBlockSize;     // +151 when nonzero, size of User block for ID block versions 1,2,3,4
    uint16_t userBlockLoc;      // +153 when nonzero, offset to User block start from ID block versions 1,2,3,4 start (below ID block)
    uint16_t idBlockCRC;        // +155 CRC of SysIDBlock with idBlockCRC member reset to zero and base CRC value of zero, and excluding idBlock2 and reserved members
    uint8_t  marker[6];         // +157 valid if [0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa]
} SysIDBlockType;

#define _SysIDBlockType_size    163
_Static_assert(sizeof(SysIDBlockType) == _SysIDBlockType_size, "Incorrect _SysIDBlockType size");

typedef struct {
    char  flashXPC;             // +0   XPC used to access flash via XMEM (e000-ffff)
    int   sectorSize;           // +1   sector size in bytes (=128 for large-sector flash)
    int   numSectors;           // +3   number of sectors on the flash
    int   flashSize;            // +5   size of the flash in 4KB blocks
    char  writeMode;            // +7   method of data-writing (currently only 1-3, 0x1x)
    void *eraseChipPtr;         // +8   pointer to erase chip function in RAM
    void *writePtr;             // +10  pointer to write flash sector function in RAM
} _FlashInfoType;

#define _FlashInfoType_size     12
_Static_assert(sizeof(_FlashInfoType) == _FlashInfoType_size, "Incorrect _FlashInfoType size");

typedef uint32_t faraddr_t;

#endif
