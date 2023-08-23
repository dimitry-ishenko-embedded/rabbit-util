/*
   Copyright (c) 2015 Digi International Inc.

   This Source Code Form is subject to the terms of the Mozilla Public
   License, v. 2.0. If a copy of the MPL was not distributed with this
   file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef SYSIODEFS_H
#define SYSIODEFS_H

#define R2000       0x0000
#define R3000       0x0100
#define R4000       0x0200

#define REV0        0x00
#define REV1        0x01
#define REV2        0x02
#define REV3        0x03
                                    // Processor    Code on chip
#define R2000_R0    (R2000|REV0)    // 2000         IQ2T
#define R2000_R1    (R2000|REV1)    // 2000         IQ3T
#define R2000_R2    (R2000|REV2)    // 2000         IQ4T
#define R2000_R3    (R2000|REV3)    // 2000         IQ5T
#define R3000_R0    (R3000|REV0)    // 3000         IL1T or IZ1T
#define R3000_R1    (R3000|REV1)    // 3000A        IL2T or IZ2T
#define R4000_R0    (R4000|REV0)    // 4000         ????

// Masks to retrieve CPU ID or Revision number
#define CPU_ID_MASK(x) (x & 0x1f00)
#define CPU_REV_MASK(x) (x & 0x1f)

/*
 * The _RAB4K macro is intended to be used for code which uses Rabbit 4000
 * instructions _RAB4KPERI is intended for when a Rabbit4000 is being used just
 * for its peripherals i.e. it _may_ be running in R3000 compatibility mode.
 * Generally, if _RAB4KPERI is used for conditional compilation, then tests for
 * _RAB4K should be _within_ those sections (the other way around is pretty
 * useless).
 */
#if CPU_ID_MASK(_CPU_ID_) == R4000
    #define _RAB4K 1
    #define _RAB4KPERI 1
#else
    #define _RAB4K 0
    #ifndef _RAB4KPERI
        #define _RAB4KPERI 0 // Default to assuming is not a Rabbit 4000.
    #endif
#endif

// Registers
#define GCSR        0x00    // Global control/status register
#define RTCCR       0x01    // Real time clock control register
#define RTC0R       0x02    // Real time clock register 0
#define RTC1R       0x03    // Real time clock register 0
#define RTC2R       0x04    // Real time clock register 0
#define RTC3R       0x05    // Real time clock register 0
#define RTC4R       0x06    // Real time clock register 0
#define RTC5R       0x07    // Real time clock register 0
#define WDTCR       0x08    // Watch-dog timer control register
#define WDTTR       0x09    // Watch-dog timer test register
#define GCM0R       0x0a    // Global clock modulator register 0
#define GCM1R       0x0b    // Global clock modulator register 1
#define GPSCR       0x0d    // Global power save control register
#define GOCR        0x0e    // Global output control register
#define GCDR        0x0f    // Global clock double register

#define MMIDR       0x10    // MMU program offset register
#define STACKSEG    0x11    // MMU stack base register
#define DATASEG     0x12    // MMU data base register
#define SEGSIZE     0x13    // MMU common bank area register
#define MB0CR       0x14    // Memory bank 0 control register
#define MB1CR       0x15    // Memory bank 1 control register
#define MB2CR       0x16    // Memory bank 2 control register
#define MB3CR       0x17    // Memory bank 3 control register
#define MECR        0x18    // MMU expanded code register
#define MTCR        0x19    // Memory timing control register
#define BDCR        0x1c    // Breakpoint/debug control register

#define SPD0R       0x20    // Slave port data 0 register
#define SPD1R       0x21    // Slave port data 1 register
#define SPD2R       0x22    // Slave port data 2 register
#define SPSR        0x23    // Slave port status register
#define SPCR        0x24    // Slave port control register
#define GROM        0x2c    // Global ROM configuration register
#define GRAM        0x2d    // Global RAM configuration register
#define GCPU        0x2e    // Global CPU configuration register
#define GREV        0x2f    // Global revision register

#define PADR        0x30    // Port A data register

#define PFDR        0x38    // Port F data register
#define PFCR        0x3c    // Port F control register
#define PFFR        0x3d    // Port F function register
#define PFDCR       0x3e    // Port F drive control register
#define PFDDR       0x3f    // Port F data direction register

#define PBDR        0x40    // Port B data register
#define PBDDR       0x47    // Port B data direction register

#define PGDR        0x48    // Port G data register
#define PGCR        0x4c    // Port G control register
#define PGFR        0x4d    // Port G function register
#define PGDCR       0x4e    // Port G drive control register
#define PGDDR       0x4f    // Port G data direction register

#define PCDR        0x50    // Port C data register
#define PCFR        0x55    // Port C function register

#define ICCSR       0x56    // Input Capture Control/Status Register
#define ICCR        0x57    // Input Capture Control Register
#define ICT1R       0x58    // Input Capture Channel 1 Trigger Register
#define ICS1R       0x59    // Input Capture Channel 1 Source Register
#define ICL1R       0x5a    // Input Capture Channel 1 LSB
#define ICM1R       0x5b    // Input Capture Channel 1 MSB
#define ICT2R       0x5c    // Input Capture Channel 2 Trigger Register
#define ICS2R       0x5d    // Input Capture Channel 2 Source Register
#define ICL2R       0x5e    // Input Capture Channel 2 LSB
#define ICM2R       0x5f    // Input Capture Channel 2 MSB

#define PDDR        0x60    // Port D data register
#define PDCR        0x64    // Port D control register
#define PDFR        0x65    // Port D function register
#define PDDCR       0x66    // Port D drive control register
#define PDDDR       0x67    // Port D data direction register
#define PDB0R       0x68    // Port D bit 0 register
#define PDB1R       0x69    // Port D bit 1 register
#define PDB2R       0x6a    // Port D bit 2 register
#define PDB3R       0x6b    // Port D bit 3 register
#define PDB4R       0x6c    // Port D bit 4 register
#define PDB5R       0x6d    // Port D bit 5 register
#define PDB6R       0x6e    // Port D bit 6 register
#define PDB7R       0x6f    // Port D bit 7 register

#define PEDR        0x70    // Port E data register
#define PECR        0x74    // Port E control register
#define PEFR        0x75    // Port E function register
#define PEDDR       0x77    // Port E data direction register
#define PEB0R       0x78    // Port E bit 0 register
#define PEB1R       0x79    // Port E bit 1 register
#define PEB2R       0x7a    // Port E bit 2 register
#define PEB3R       0x7b    // Port E bit 3 register
#define PEB4R       0x7c    // Port E bit 4 register
#define PEB5R       0x7d    // Port E bit 5 register
#define PEB6R       0x7e    // Port E bit 6 register
#define PEB7R       0x7f    // Port E bit 7 register

#define IB0CR       0x80    // I/O bank 0 control register
#define IB1CR       0x81    // I/O bank 1 control register
#define IB2CR       0x82    // I/O bank 2 control register
#define IB3CR       0x83    // I/O bank 3 control register
#define IB4CR       0x84    // I/O bank 4 control register
#define IB5CR       0x85    // I/O bank 5 control register
#define IB6CR       0x86    // I/O bank 6 control register
#define IB7CR       0x87    // I/O bank 7 control register

#define PWL0R       0x88    // PWM channel 0 LSB register
#define PWM0R       0x89    // PWM channel 0 MSB register
#define PWL1R       0x8a    // PWM channel 1 LSB register
#define PWM1R       0x8b    // PWM channel 1 MSB register
#define PWL2R       0x8c    // PWM channel 2 LSB register
#define PWM2R       0x8d    // PWM channel 2 MSB register
#define PWL3R       0x8e    // PWM channel 3 LSB register
#define PWM3R       0x8f    // PWM channel 3 MSB register

#define QDCSR       0x90    // Quadrature decode control/status register
#define QDCR        0x91    // Quadrature decode control register
#define QDC1R       0x94    // Quadrature decode channel 1 count
#define QDC1HR      0x95    // Quadrature decode channel 1 count
#define QDC2R       0x96    // Quadrature decode channel 2 count
#define QDC2HR      0x97    // Quadrature decode channel 2 count

#define I0CR        0x98    // Interrupt 0 control register
#define I1CR        0x99    // Interrupt 1 control register

#define TACSR       0xa0    // Timer A control/status register
#define TAPR        0xa1    // Timer A prescale register
#define TAT1R       0xa3    // Timer A time constant 1 register
#define TACR        0xa4    // Timer A control register
#define TAT2R       0xa5    // Timer A time constant 2 register
#define TAT8R       0xa6    // Timer A time constant 8 register
#define TAT3R       0xa7    // Timer A time constant 3 register
#define TAT9R       0xa8    // Timer A time constant 9 register
#define TAT4R       0xa9    // Timer A time constant 4 register
#define TAT10R      0xaa    // Timer A time constant 10 register
#define TAT5R       0xab    // Timer A time constant 5 register
#define TAT6R       0xad    // Timer A time constant 6 register
#define TAT7R       0xaf    // Timer A time constant 7 register

#define TBCSR       0xb0    // Timer B control/status register
#define TBCR        0xb1    // Timer B control register
#define TBM1R       0xb2    // Timer B match B1 register, MSB
#define TBL1R       0xb3    // Timer B match B1 register, LSB
#define TBM2R       0xb4    // Timer B match B2 register, MSB
#define TBL2R       0xb5    // Timer B match B2 register, LSB
#define TBCMR       0xbe    // Timer B current count register, MSB
#define TBCLR       0xbf    // Timer B current count register, LSB

#define SADR        0xc0    // Serial port A data register
#define SAAR        0xc1    // Serial port A address register
#define SALR        0xc2    // Serial port A long stop register
#define SASR        0xc3    // Serial port A status register
#define SACR        0xc4    // Serial port A control register
#define SAER        0xc5    // Serial port A extended register

#define SEDR        0xc8    // Serial port E data register
#define SEAR        0xc9    // Serial port E address register
#define SELR        0xca    // Serial port E long stop register
#define SESR        0xcb    // Serial port E status register
#define SECR        0xcc    // Serial port E control register
#define SEER        0xcd    // Serial port E extended register

#define SBDR        0xd0    // Serial port B data register
#define SBAR        0xd1    // Serial port B address register
#define SBLR        0xd2    // Serial port B long stop register
#define SBSR        0xd3    // Serial port B status register
#define SBCR        0xd4    // Serial port B control register
#define SBER        0xd5    // Serial port B extended register

#define SFDR        0xd8    // Serial port F data register
#define SFAR        0xd9    // Serial port F address register
#define SFLR        0xda    // Serial port F long stop register
#define SFSR        0xdb    // Serial port F status register
#define SFCR        0xdc    // Serial port F control register
#define SFER        0xdd    // Serial port F extended register

#define SCDR        0xe0    // Serial port C data register
#define SCAR        0xe1    // Serial port C address register
#define SCLR        0xe2    // Serial port C long stop register
#define SCSR        0xe3    // Serial port C status register
#define SCCR        0xe4    // Serial port C control register
#define SCER        0xe5    // Serial port C extended register

#define SDDR        0xf0    // Serial port D data register
#define SDAR        0xf1    // Serial port D address register
#define SDLR        0xf2    // Serial port D long stop register
#define SDSR        0xf3    // Serial port D status register
#define SDCR        0xf4    // Serial port D control register
#define SDER        0xf5    // Serial port D extended register

#if _CPU_ID_ >= R3000_R1

#define RTUER       0x300   // Real Time Clock User Enable Register
#define SPUER       0x320   // Slave Port User Enable Register
#define PAUER       0x330   // Parallel Port A User Enable Register
#define PBUER       0x340   // Parallel Port B User Enable Register
#define PCUER       0x350   // Parallel Port C User Enable Register
#define PDUER       0x360   // Parallel Port D User Enable Register
#define PEUER       0x370   // Parallel Port E User Enable Register
#define PFUER       0x338   // Parallel Port F User Enable Register
#define PGUER       0x348   // Parallel Port G User Enable Register
#define ICUER       0x358   // Input Capture User Enable Register
#define IBUER       0x380   // I/O Bank User Enable Register
#define PWUER       0x388   // PWM User Enable Register
#define QDUER       0x390   // Quad Decode User Enable Register
#define IUER        0x398   // External Interrupt User Enable Register
#define TAUER       0x3a0   // Timer A User Enable Register
#define TBUER       0x3b0   // Timer B User Enable Register
#define SAUER       0x3c0   // Serial Port A User Enable Register
#define SBUER       0x3d0   // Serial Port B User Enable Register
#define SCUER       0x3e0   // Serial Port C User Enable Register
#define SDUER       0x3f0   // Serial Port D User Enable Register
#define SEUER       0x3c8   // Serial Port E User Enable Register
#define SFUER       0x3d8   // Serial Port F User Enable Register

#define EDMR        0x420   // Enable Dual-Mode Register
#define WPCR        0x440   // Write Protect Control Register

#define STKCR       0x444   // Stack Limit Control Register
#define STKLLR      0x445   // Stack Low Limit Register
#define STKHLR      0x446   // Stack High Limit Register

#define RAMSR       0x448   // RAM Segment Register

#define WPLR        0x460
#define WPHR        0x461

#define WPSAR       0x480
#define WPSALR      0x481
#define WPSAHR      0x482
#define WPSBR       0x484
#define WPSBLR      0x485
#define WPSBHR      0x486

#if __RABBITSYS

// Internal interrupt vectors register value equivalents
#define IVINT_BASE  0x4b0
#define IV_VALUE(X, BASE) ((X>>4) + BASE)

#define IVPER       IVINT_VALUE(PERIODIC_OFS, IVINT_BASE)
#define IVSECWD     IVINT_VALUE(SECWD_OFS, IVINT_BASE)
#define IVRST10     IVINT_VALUE(RST10_OFS, IVINT_BASE)
#define IVRST18     IVINT_VALUE(RST18_OFS, IVINT_BASE)
#define IVRST20     IVINT_VALUE(RST20_OFS, IVINT_BASE)
#define IVRST28     IVINT_VALUE(RST28_OFS, IVINT_BASE)
#define IVSYSCALL   IVINT_VALUE(SYSCALL_OFS, IVINT_BASE)
#define IVRST38     IVINT_VALUE(RST38_OFS, IVINT_BASE)
#define IVSLAVE     IVINT_VALUE(SLAVE_OFS, IVINT_BASE)
#define IVWPV       IVINT_VALUE(WPV_OFS, IVINT_BASE)
#define IVTIMERA    IVINT_VALUE(TIMERA_OFS, IVINT_BASE)
#define IVTIMERB    IVINT_VALUE(TIMERB_OFS, IVINT_BASE)
#define IVSERA      IVINT_VALUE(SERA_OFS, IVINT_BASE)
#define IVSERB      IVINT_VALUE(SERB_OFS, IVINT_BASE)
#define IVSERC      IVINT_VALUE(SERC_OFS, IVINT_BASE)
#define IVSERD      IVINT_VALUE(SERD_OFS, IVINT_BASE)
#define IVPWM       IVINT_VALUE(PWM_OFS, IVINT_BASE)
#define IVSMV       IVINT_VALUE(SMV_OFS, IVINT_BASE)
#define IVQUAD      IVINT_VALUE(QUAD_OFS, IVINT_BASE)
#define IVINPUTCAP  IVINT_VALUE(INPUTCAP_OFS, IVINT_BASE)
#define IVSLV       IVINT_VALUE(SLV_OFS, IVINT_BASE)
#define IVSERE      IVINT_VALUE(SERE_OFS, IVINT_BASE)
#define IVSERF      IVINT_VALUE(SERF_OFS, IVINT_BASE)

// External interrupt vectors register value equivalents
#define IVEXT_BASE  0x4e0
#define IVEXT0      IVINT_VALUE(EXT0_OFS, IVEXT_BASE)
#define IVEXT1      IVINT_VALUE(EXT1_OFS, IVEXT_BASE)

#endif

#endif

#define SS_RRDY_BIT  7      // Serial port status read buffer ready bit
#define SS_ADDR_BIT  6      // Serial port status address byte in buffer bit
#define SS_OVRN_BIT  5      // Serial port status overrun bit
#define SS_TFULL_BIT 3      // Serial port status transmit buffer full bit
#define SS_TPRG_BIT  2      // Serial port status transmit in progress bit

#define SWDTR        0x0c   // Secondary watchdog timer register

// Internal interrupts and their offset from INTVEC_BASE
#define PERIODIC_OFS 0x00
#define SECWD_OFS    0x10
#define RST10_OFS    0x20
#define RST18_OFS    0x30
#define RST20_OFS    0x40
#define RST28_OFS    0x50
#define SYSCALL_OFS  0x60
#define RST38_OFS    0x70
#define SLAVE_OFS    0x80
#define WPV_OFS      0x90   // Write Protect Violation
#define TIMERA_OFS   0xa0
#define TIMERB_OFS   0xb0
#define SERA_OFS     0xc0
#define SERB_OFS     0xd0
#define SERC_OFS     0xe0
#define SERD_OFS     0xf0
// The following can be co-located in the same 256 byte block as external interrupts
#define PWM_OFS      0x170
#define SMV_OFS      0x180  // System Mode Violation
#define QUAD_OFS     0x190
#define INPUTCAP_OFS 0x1a0
#define SLV_OFS      0x1b0  // Stack Limit Violation
#define SERE_OFS     0x1c0
#define SERF_OFS     0x1d0

// External interrupts and their offset from XINTVEC_BASE
#define EXT0_OFS     0x00
#define EXT1_OFS     0x10

#define _OP_JP       0xc3
#define _OP_RET      0xc9

#endif
