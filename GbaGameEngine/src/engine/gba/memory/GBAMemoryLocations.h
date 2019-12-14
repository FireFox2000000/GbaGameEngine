#pragma once

//! Put variable in IWRAM (default).
#define IWRAM_DATA __attribute__((section(".iwram")))

//! Put variable in EWRAM.
#define EWRAM_DATA __attribute__((section(".ewram")))

//! Put <b>non</b>-initialized variable in EWRAM.
#define  EWRAM_BSS __attribute__((section(".sbss")))

//! Put function in IWRAM.
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

//! Put function in EWRAM.
#define EWRAM_CODE __attribute__((section(".ewram"), long_call))

#define IO_REGISTERS (0x4000000)
#define IO_REGISTERS_END (0x040003FF)

#define COLOUR_PALETTE_RAM (0x05000000)
#define COLOUR_PALETTE_RAM_END (0x050003FF)

#define VRAM (0x06000000)
#define VRAM_END (0x06017FFF)

#define OAM_RAM (0x07000000)
#define OAM_RAM_END (0x070003FF)
