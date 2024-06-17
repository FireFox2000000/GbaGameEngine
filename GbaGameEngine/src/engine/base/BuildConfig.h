#pragma once

#ifdef DEBUG

#define ENABLE_LOGGING

#elif defined(PROFILE)

#define ENABLE_LOGGING

#endif

#if defined(ENABLE_LOGGING) || defined(DEBUG)
#define VBA_GBA
//#define NOCASH_GBA
#endif