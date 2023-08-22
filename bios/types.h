/*
 * Copyright (c) 2015 Digi International Inc.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __TYPES_H
#define __TYPES_H

#include <stdint.h>

typedef struct _SysIDBlockType2 {
    uint8_t  flashMBC;          // memory bank configurations
    uint8_t  flash2MBC;
    uint8_t  ramMBC;
    uint32_t devSpecLoc;        // count of additional memory devices immediately preceding this block
    uint32_t macrosLoc;         // start of the macro table for additional board configuration options
    uint32_t driversLoc;        // offset to preloaded drivers start from ID block start (positive is below ID block)
    uint32_t ioDescLoc;         // offset to I/O descriptions start from ID block start (positive is below ID block)
    uint32_t ioPermLoc;         // offset to User mode I/O permissions start from ID block start (positive is below ID block)
    uint32_t persBlockLoc;      // offset to persistent storage block area start from ID block start (positive is below ID block)
    uint16_t userBlockSiz2;     // size of v5 "new style" mirrored User block image
    uint16_t idBlockCRC2;       // CRC of SysIDBlockType2 type with idBlockCRC2 member reset to zero and base CRC value of SysIDBlock.idBlockCRC
} SysIDBlockType2;

typedef struct _SysIDBlockType {
    uint16_t tableVersion;      // version number for this table layout
    uint16_t productID;         // Z-World part #
    uint16_t vendorID;          // 1 = Z-World
    uint8_t  timestamp[7];      // yy/m/d h:m:s
    uint32_t flashID;           // Z-World part # dev on CS0 OE0 (normally primary flash)
    uint16_t flashType;         // write method
    uint16_t flashSize;         // in 1000h pages
    uint16_t sectorSize;        // size of flash sector in bytes
    uint16_t numSectors;        // number of sectors
    uint16_t flashSpeed;        // in nanoseconds
    uint32_t flash2ID;          // Z-World part #, 2nd flash
    uint16_t flash2Type;        // write method, 2nd flash
    uint16_t flash2Size;        // in 1000h pages, 2nd flash
    uint16_t sector2Size;       // size of 2nd flash's sectors in bytes
    uint16_t num2Sectors;       // number of sectors
    uint16_t flash2Speed;       // in nanoseconds, 2nd flash
    uint32_t ramID;             // Z-World part #
    uint16_t ramSize;           // in 1000h pages
    uint16_t ramSpeed;          // in nanoseconds
    uint16_t cpuID;             // CPU type identification
    uint32_t crystalFreq;       // in Hertz
    uint8_t  macAddr[6];        // MAC address
    uint8_t  serialNumber[24];  // device serial number
    uint8_t  productName[30];   // null-terminated string

    SysIDBlockType2 idBlock2;   // idblock
    uint8_t  reserved[1];       // allow future expansion (eg, see addition of v5's idblock2 member structure, above)
    uint32_t idBlockSize;       // size of versions 1,2,3,4 ID block
    uint16_t userBlockSize;     // when nonzero, size of User block for ID block versions 1,2,3,4
    uint16_t userBlockLoc;      // when nonzero, offset to User block start from ID block versions 1,2,3,4 start (below ID block)
    uint16_t idBlockCRC;        // CRC of SysIDBlock with idBlockCRC member reset to zero and base CRC value of zero, and excluding idBlock2 and reserved members
    uint8_t  marker[6];         // valid if [0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa]
} SysIDBlockType;

typedef struct {
    char  flashXPC;             // XPC used to access flash via XMEM (e000-ffff)
    int   sectorSize;           // sector size in bytes (=128 for large-sector flash)
    int   numSectors;           // number of sectors on the flash
    int   flashSize;            // size of the flash in 4KB blocks
    char  writeMode;            // method of data-writing (currently only 1-3, 0x1x)
    void *eraseChipPtr;         // pointer to erase chip function in RAM
    void *writePtr;             // pointer to write flash sector function in RAM
} _FlashInfoType;

typedef uint32_t faraddr_t;

#endif
