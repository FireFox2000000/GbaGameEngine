#pragma once

#define GBATEK_ALIGN(n) __attribute__((aligned(n)))

#if	defined	(__thumb__)
#define	GBATEK_BiosSystemCall(Number)	 __asm ("SWI	  "#Number"\n" :::  "r0", "r1", "r2", "r3")
#else
#define	GBATEK_BiosSystemCall(Number)	 __asm ("SWI	  "#Number"	<< 16\n" ::: "r0", "r1", "r2", "r3")
#endif

#ifdef __GNUC__

#define GBATEK_DIAGNOSTIC_PUSH_IGNORE_WARNING_ANON_STRUCTS \
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wpedantic\"")

#define GBATEK_DIAGNOSTIC_PUSH_IGNORED_MISSING_RETURN \
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wreturn-type\"")

#define GBATEK_DIAGNOSTIC_POP _Pragma ("GCC diagnostic pop") 

#define GBATEK_OPTIONS_PUSH_OPTIMIZE_O0 \
	_Pragma("GCC push_options")\
	_Pragma("GCC optimize (\"O0\")")\

#define GBATEK_OPTIONS_POP _Pragma("GCC pop_options")

#else
#define GBATEK_DIAGNOSTIC_PUSH_IGNORE_WARNING_ANON_STRUCTS
#define GBATEK_DIAGNOSTIC_PUSH_IGNORED_MISSING_RETURN
#define GBATEK_DIAGNOSTIC_POP

#define GBATEK_OPTIONS_PUSH_OPTIMIZE_O0
#define GBATEK_OPTIONS_POP
#endif

#define GBATEK_KILOBYTES_TO_BYTES(kbytes) (static_cast<int>((kbytes) * 1024))
