#pragma once

#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/base/BuildConfig.h"

namespace Debug
{
	void Log(const char* message);
	void LogFormat(const char* format, ...);
	void LogAtLocation(const char* file, const int line, const char* format, ...);
}

#ifdef ENABLE_LOGGING

	#ifdef LOG_LOCATION_BY_DEFAULT
		#define DEBUG_LOG(message) { Debug::LogAtLocation(__FILE__, __LINE__, message); }
		#define DEBUG_LOGFORMAT(format, ...) { Debug::LogAtLocation(__FILE__, __LINE__, format, __VA_ARGS__); }
	#else
		#define DEBUG_LOG(message) { Debug::Log(message); }
		#define DEBUG_LOGFORMAT(format, ...) { Debug::LogFormat(format, __VA_ARGS__); }
	#endif

#else

#define DEBUG_LOG(message)
#define DEBUG_LOGFORMAT(format, ...)

#endif
