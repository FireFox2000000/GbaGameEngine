#pragma once

//! Put variable in IWRAM (default)
#define IWRAM_DATA __attribute__((section(".iwram")))

//! Put variable in EWRAM.
#define EWRAM_DATA __attribute__((section(".ewram")))

//! Put <b>non</b>-initialized variable in EWRAM.
#define EWRAM_BSS __attribute__((section(".sbss")))

//! Put function in IWRAM.
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

//! Put function in EWRAM.
#define EWRAM_CODE __attribute__((section(".ewram"), long_call))

#define ARM __attribute__((__target__("arm")))
#define THUMB __attribute__((__target__("thumb")))

#define ARM_IWRAM_CODE ARM IWRAM_CODE
