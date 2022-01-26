#pragma once

// Retail will be defined by makefile CFLAGS for final builds.
#ifndef RETAIL

#define DEBUG
//#define PROFILE

#endif

#ifdef DEBUG

//#define NOCASH_GBA
#define VBA_GBA
#define ENABLE_LOGGING
//#define LOG_LOCATION_BY_DEFAULT

#elif defined(PROFILE)

#define NOCASH_GBA
#define ENABLE_LOGGING

#else

#endif