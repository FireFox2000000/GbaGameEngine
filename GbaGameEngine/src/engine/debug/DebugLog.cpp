#include "DebugLog.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include "engine/base/Macros.h"

#ifdef NOCASH_GBA
#include "engine/gba/tonc/include/tonc_nocash.h"

using namespace std;

inline void VLogFormat(const char* format, va_list args)
{
	char buffer[256];
	vsprintf(buffer, format, args);
	Debug::Log(buffer);
}

#define VLOGFORMAT_W_UNPACK(vaargStart, message) \
	{\
		va_list argptr;\
		va_start(argptr, (vaargStart));\
		VLogFormat((message), argptr);\
		va_end(argptr);\
	}

void Debug::Log(const char * message)
{
	nocash_puts(message);
}

void Debug::LogFormat(const char * format, ...)
{
	VLOGFORMAT_W_UNPACK(format, format);
}

void Debug::LogAtLocation(const char * file, const int line, const char * format, ...)
{
	// Output this on two different lines with tab spacing for better formatting to no$gba debugger
	{
		Debug::LogFormat("%s(%d): ", file, line);
	}

	{
		//string message;
		//message += "    ";
		//message += format;

		VLOGFORMAT_W_UNPACK(format, format);
	}
}

void Debug::LogAssertionFailure(const char* file, const int line, const char* format, ...)
{
	Log("ASSERTION FAILED!");

	va_list argptr;
	va_start(argptr, (format));
	LogAtLocation(file, line, format, argptr);
	va_end(argptr);
}

#undef VLOGFORMAT_W_UNPACK

#endif

// Not implemented
