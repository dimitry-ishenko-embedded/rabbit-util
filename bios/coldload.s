;
; Copyright (c) 2023 Dimitry Ishenko <dimitry.ishenko (at) (gee) mail (dot) com>
; Copyright (c) 2020 Digi International Inc.
;
; This Source Code Form is subject to the terms of the Mozilla Public
; License, v. 2.0. If a copy of the MPL was not distributed with this
; file, You can obtain one at http://mozilla.org/MPL/2.0/.
;
    .module coldload

DIVADDR  .equ 0x3f00 ; time constant address
FREQADRS .equ 0x3f02 ; frequency divisor address

DATASEG  .equ 0x0012
PCFR     .equ 0x0055
RTC0R    .equ 0x0002
SACR     .equ 0x00c4
SADR     .equ 0x00c0
SASR     .equ 0x00c3
SEGSIZE  .equ 0x0013
TACSR    .equ 0x00a0
TAT4R    .equ 0x00a9
WDTCR    .equ 0x0008
WDTTR    .equ 0x0009

    .area SYS (ABS)
    .org 0

    ld sp, #(coldloadend + 0x200)   ; set up stack in low root segment

; Start of crystal frequency detection.
    ld bc, #0x0000                  ; our counter
    ld de, #0x07ff                  ; mask for RTC bits

; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; ! WARNING: Time critical code for crystal frequency       !
; ! detection begins here. Adding or removing code from the !
; ! following loops will affect the frequency computation.  !
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

wait_for_zero:
    ioi
    ld (RTC0R), a                   ; fill RTC registers
    ioi
    ld hl, (RTC0R)                  ; get lowest two RTC regs
    and hl, de                      ; mask off bits
    jr nz, wait_for_zero            ; wait until bits 0-9 are zero

timing_loop:
    inc bc                          ; increment counter
    push bc                         ; save counter
    ld b, #0x98                     ; empirical loop value (timed for 2 wait states)
    ld hl, #WDTCR

delay_loop:
    ioi
    ld (hl), #0x5a                  ; hit watchdog
    djnz delay_loop
    pop bc                          ; restore counter
    ioi
    ld (RTC0R), a                   ; fill RTC registers
    ioi
    ld hl, (RTC0R)                  ; get lowest two RTC regs
    bit 2, h                        ; test bit 10
    jr z, timing_loop               ; repeat until bit set

; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; ! Time critical code for crystal frequency detection ends    !
; ! here.                                                      !
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ld h, b
    ld l, c
    ld de, #0x0008
    add hl, de                      ; add 8 (equiv. to rounding up later)

    rr hl
    rr hl
    rr hl
    rr hl                           ; divide by 16
    ld a, l                         ; this is our divider!

    dec a
    ioi
    ld (TAT4R), a                   ; set timer A4 running at 57600 baud
    inc a

    ld b, a
    sla a
    add a, b                        ; multiply by 3 to get 19200 baud

    ld (FREQADRS), a                ; save divisor for later
    dec a

    ld (DIVADDR), a                 ; save 19200 baud scaling
    ld a, #0x01
    ioi
    ld (TACSR), a                   ; enable timer A with cpuclk/2
    xor a
    ioi
    ld (SACR), a                    ; set serial port A async, 8 bit, parallel port C input
    ld a, #0x51
    ioi
    ld (WDTTR), a                   ; disable the watchdog timer
    ld a, #0x54
    ioi
    ld (WDTTR), a                   ; disable the watchdog timer
    ld a, #0x40
    ioi
    ld (PCFR), a

    call get_byte
    ld e, a                         ; pilot BIOS begin physical address LSB

    call get_byte
    ld d, a                         ; pilot BIOS begin physical address LSmidB

    call get_byte
    ld c, a                         ; pilot BIOS begin physical address MSmidB

    call get_byte
    ld b, a                         ; pilot BIOS begin physical address MSB

    call get_byte
    ld l, a                         ; pilot BIOS size LSB

    call get_byte
    ld h, a                         ; pilot BIOS size MSB

    call get_byte
    altd
    ld a, a                         ; store received checksum in alt A

    ld a, e                         ; initialize and calculate local checksum
    add a, d
    add a, c
    add a, b
    add a, l
    add a, h
    call send_byte                  ; send ack echoing the locally calculated checksum

    exx
    ld b, a
    ex af, af'                      ; get received checksum
    cp b                            ; compare checksums
    jp nz, timeout                  ; if checksums do not match error out

    exx
    push hl                         ; save pilot BIOS size
    ld h, c                         ; copy pilot BIOS begin physical address middle
    ld l, d                         ; bytes into HL
    rr hl                           ; shift physical address bits 19:12 into L
    rr hl
    rr hl
    rr hl
    ld a, l                         ; copy pilot BIOS physical address bits 19:12 into A
    sub #0x06                       ; calculate DATASEG value for pilot at 0x6000 logical
    ioi
    ld (DATASEG), a
    ld a, #0xe6                     ; no stack seg (0xe000), put data seg boundary at 0x6000
    ioi
    ld (SEGSIZE), a

    ld a, d                         ; copy pilot BIOS physical address LSmidB into A
    and #0x0f                       ; change upper nibble of LSmidB to 0x6x
    or #0x60
    ld h, a                         ; copy the 0x6xxx logical address into HL
    ld l, e

    ld a, l

    pop de                          ; recover the pilot BIOS size into DE
    ld iy, hl                       ; save pilot BIOS logical begin in IY for copy-to-RAM index
    ld ix, hl                       ; and in IX for the jump to the pilot BIOS

wait_for_cc:
    call get_byte
    cp #0xcc                        ; initial pilot BIOS code (flag) byte?
    jr nz, wait_for_cc
    xor a
    ld (iy), a                      ; replace the 0xcc marker with 0x00 (nop)
    inc iy                          ; increment the copy-to-RAM index
    dec de                          ; one less byte to copy
    ld bc, #0xcccc                  ; update the (initially 0x0000) 8-bit Fletcher
                                    ; checksum value with the 0xcc just received

load_pilot_loop:
    call get_byte
    ld (iy), a

; Use 8-bit Fletcher checksum algorithm. See RFC1145 for more info.
    add a, b
    adc a, #0x00
    ld b, a                         ; A = A + D[i]
    add a, c
    adc a, #0x00
    ld c, a                         ; B = B + A

    inc iy                          ; increment the copy-to-RAM index
    dec de                          ; one less byte to copy
    bool hl
    ld l, h                         ; zero hl
    or hl, de                       ; check remaining size of pilot
    jr nz, load_pilot_loop          ; repeat until size bytes are received

    ld a, c                         ; send LSB of pilot BIOS Fletcher checksum
    call send_byte
    ld a, b                         ; send MSB of pilot BIOS Fletcher checksum
    call send_byte

;   ioi ld (WDTTR), a               ; reenable the watchdog timer

    jp (ix)                         ; start running pilot bios

get_byte::
pollrxbuf:
    ioi
    ld a, (SASR)                    ; check byte receive status
    bit 7, a
    jr z, pollrxbuf                 ; wait until byte received
    ioi
    ld a, (SADR)                    ; get byte
    ret

send_byte::
; Must not destroy register A!
    exx
polltxbuf:
    ld hl, #SASR
    ioi
    bit 3, (hl)
    jr nz, polltxbuf                ; wait for serial port A not busy
    ioi
    ld (SADR), a                    ; send byte
    exx
    ret

timeout::
    ld e, #0x55
    jr timeout

coldloadend::
