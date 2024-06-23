#pragma once

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

#define ARRAY_SIZE(TArray) (sizeof((TArray)) / sizeof(*(TArray)))

#ifndef NULL
#	define NULL nullptr
#endif

#if defined(DEBUG)
#include "engine/debug/DebugLog.h"
#endif

#ifdef Platform_NoCash
#include "engine/emulator/NoCashEmulator.h"
#define BREAKPOINT() NoCashEmulator::NativeBreakpoint()
#else
#define BREAKPOINT()
#endif

#ifdef DEBUG
#ifdef LOG_LOCATION_BY_DEFAULT
#define DEBUG_LOG(message) { Debug::LogAtLocation(__FILE__, __LINE__, message); }
#define DEBUG_LOGFORMAT(format, ...) { Debug::LogAtLocation(__FILE__, __LINE__, format, __VA_ARGS__); }
#define DEBUG_ERROR(message) { Debug::LogAssertionFailure(__FILE__, __LINE__, (message)); BREAKPOINT(); }
#define DEBUG_ERRORFORMAT(format, ...) { Debug::LogAssertionFailure(__FILE__, __LINE__, (format), __VA_ARGS__); BREAKPOINT(); }
#define DEBUG_ASSERTMSG(condition, message) { if (!(condition)) { Debug::LogAssertionFailure(__FILE__, __LINE__, (message)); BREAKPOINT(); } }
#define DEBUG_ASSERTMSGFORMAT(condition, format, ...) { if (!(condition)) { Debug::LogAssertionFailure(__FILE__, __LINE__, (format), __VA_ARGS__); BREAKPOINT(); } }
#else
#define DEBUG_LOG(message) { Debug::Log(message); }
#define DEBUG_LOGFORMAT(format, ...) { Debug::LogFormat(format, __VA_ARGS__); }
#define DEBUG_ERROR(message) { Debug::LogAssertionFailure(__FILE__, __LINE__, (message)); BREAKPOINT(); }
#define DEBUG_ERRORFORMAT(format, ...) { Debug::LogAssertionFailure(__FILE__, __LINE__, (format), __VA_ARGS__); BREAKPOINT(); }
#define DEBUG_ASSERTMSG(condition, message) { if (!(condition)) { Debug::LogAssertionFailure(__FILE__, __LINE__, (message)); BREAKPOINT(); } }
#define DEBUG_ASSERTMSGFORMAT(condition, format, ...) { if (!(condition)) { Debug::LogAssertionFailure(__FILE__, __LINE__, (format), __VA_ARGS__); BREAKPOINT(); } }
#endif

#else

#define DEBUG_LOG(message)
#define DEBUG_LOGFORMAT(format, ...)
#define DEBUG_ERROR(message)
#define DEBUG_ERRORFORMAT(format, ...)
#define DEBUG_ASSERTMSG(condition, message)
#define DEBUG_ASSERTMSGFORMAT(condition, format, ...)

#endif

#ifdef DEBUG
#define FATAL_ERROR() { DEBUG_LOG("Forcing game crash. Check logs."); int i = 0; DEBUG_LOGFORMAT("%d", 1 / i); }

#else
#define FATAL_ERROR()

#endif
