#pragma once

#include "engine/gba/memory/GBAMemoryLocations.h"

namespace Debug
{
	void Log(const char* message);
	void LogFormat(const char* format, ...);
	void LogAtLocation(const char* file, const int line, const char* format, ...);
	void LogAssertionFailure(const char* file, const int line, const char* format, ...);
}
