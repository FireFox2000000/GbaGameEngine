#pragma once
#include "engine/base/BuildConfig.h"
#include <type_traits>

#define UNUSED(x) ((void)(x))		// Disable compiler warnings on unused variables

// Data alignment
#define ALIGN(n) __attribute__((aligned(n)))
#define PACKED __attribute__((packed))

#define BIT(index) (1 << index)
#define SHIFTED_BITMASK(bits, index) (bits << index)	// Shift a full bitmask along to a specific index
#define BITS_U32(length) (0xFFFFFFFF >> (32 - (length)))	// Create a mask full of 1's for a specific size
#define BITS_INDEXED_U32(length, index) (SHIFTED_BITMASK(BITS_U32(length), index))	// Create a series of enabled bits starting at a specific index for a specific size
#define SET_BIT(mask, index) ((mask) |= BIT(index))
#define CLEAR_BIT(mask, index) ((mask) &= ~BIT(index))

#define STATIC_ASSERT(condition, message) static_assert(condition, message)
#define IS_BASE_OF(base, derived) (std::is_base_of<base, derived>::value)

#ifndef NULL
#	define NULL nullptr
#endif

#ifdef ENABLE_LOGGING
#include "engine/debug/DebugLog.h"

#ifdef LOG_LOCATION_BY_DEFAULT
#define DEBUG_LOG(message) { Debug::LogAtLocation(__FILE__, __LINE__, message); }
#define DEBUG_LOGFORMAT(format, ...) { Debug::LogAtLocation(__FILE__, __LINE__, format, __VA_ARGS__); }
#define DEBUG_ASSERTMSG(condition, message) { if (!(condition)) Debug::LogAssertionFailure(__FILE__, __LINE__, (message)); }
#define DEBUG_ASSERTMSGFORMAT(condition, format, ...) { if (!(condition)) Debug::LogAssertionFailure(__FILE__, __LINE__, (format), __VA_ARGS__); }
#else
#define DEBUG_LOG(message) { Debug::Log(message); }
#define DEBUG_LOGFORMAT(format, ...) { Debug::LogFormat(format, __VA_ARGS__); }
#define DEBUG_ASSERTMSG(condition, message) { if (!(condition)) Debug::LogAssertionFailure(__FILE__, __LINE__, (message)); }
#define DEBUG_ASSERTMSGFORMAT(condition, format, ...) { if (!(condition)) Debug::LogAssertionFailure(__FILE__, __LINE__, (format), __VA_ARGS__); }
#endif

#else

#define DEBUG_LOG(message)
#define DEBUG_LOGFORMAT(format, ...)
#define DEBUG_ASSERTMSG(condition, message)
#define DEBUG_ASSERTMSGFORMAT(condition, format, ...)

#endif

#ifdef DEBUG
#define FATAL_ERROR() { DEBUG_LOG("Forcing game crash. Check logs."); int i = 0; DEBUG_LOGFORMAT("%d", 1 / i); }

#else
#define FATAL_ERROR()

#endif
