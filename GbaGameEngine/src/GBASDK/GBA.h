#pragma once

//! Put variable in IWRAM (default)
#define GBA_IWRAM_DATA __attribute__((section(".iwram")))

//! Put variable in EWRAM.
#define GBA_EWRAM_DATA __attribute__((section(".ewram")))

//! Put <b>non</b>-initialized variable in EWRAM.
#define GBA_EWRAM_BSS __attribute__((section(".sbss")))

//! Put function in IWRAM.
#define GBA_IWRAM_CODE __attribute__((section(".iwram"), long_call))

//! Put function in EWRAM.
#define GBA_EWRAM_CODE __attribute__((section(".ewram"), long_call))

#define GBA_ARM __attribute__((__target__("arm")))
#define GBA_THUMB __attribute__((__target__("thumb")))

#define GBA_ARM_IWRAM_CODE GBA_ARM GBA_IWRAM_CODE

namespace GBA
{
	constexpr unsigned long SYSTEM_FREQUENCY = 0x1000000;
}