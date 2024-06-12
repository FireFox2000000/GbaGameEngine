#pragma once

//! Put variable in IWRAM (default)
#define GBATEK_IWRAM_DATA __attribute__((section(".iwram")))

//! Put variable in EWRAM.
#define GBATEK_EWRAM_DATA __attribute__((section(".ewram")))

//! Put <b>non</b>-initialized variable in EWRAM.
#define GBATEK_EWRAM_BSS __attribute__((section(".sbss")))

//! Put function in IWRAM.
#define GBATEK_IWRAM_CODE __attribute__((section(".iwram"), long_call))

//! Put function in EWRAM.
#define GBATEK_EWRAM_CODE __attribute__((section(".ewram"), long_call))

#define GBATEK_ARM __attribute__((__target__("arm")))
#define GBATEK_THUMB __attribute__((__target__("thumb")))

#define GBATEK_ARM_IWRAM_CODE GBATEK_ARM GBATEK_IWRAM_CODE

namespace GBATEK
{
	constexpr unsigned long SYSTEM_FREQUENCY = 0x1000000;
}