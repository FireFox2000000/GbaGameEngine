#pragma once

#define GBA_ALIGN(n) __attribute__((aligned(n)))

#if	defined	(__thumb__)
#define	GBA_BiosSystemCall(Number)	 __asm ("SWI	  "#Number"\n" :::  "r0", "r1", "r2", "r3")
#else
#define	GBA_BiosSystemCall(Number)	 __asm ("SWI	  "#Number"	<< 16\n" :::"r0", "r1", "r2", "r3")
#endif

#ifdef __GNUC__

#define GBA_DIAGNOSTIC_PUSH_IGNORE_WARNING_ANON_STRUCTS \
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wpedantic\"")

#define GBA_DIAGNOSTIC_PUSH_IGNORED_MISSING_RETURN \
	_Pragma("GCC diagnostic push")\
	_Pragma("GCC diagnostic ignored \"-Wreturn-type\"")

#define GBA_DIAGNOSTIC_POP _Pragma ("GCC diagnostic pop") 

#define GBA_OPTIONS_PUSH_OPTIMIZE_O0 \
	_Pragma("GCC push_options")\
	_Pragma("GCC optimize (\"O0\")")\

#define GBA_OPTIONS_POP _Pragma("GCC pop_options")

#else
#define GBA_DIAGNOSTIC_PUSH_IGNORE_WARNING_ANON_STRUCTS
#define GBA_DIAGNOSTIC_PUSH_IGNORED_MISSING_RETURN
#define GBA_DIAGNOSTIC_POP

#define GBA_OPTIONS_PUSH_OPTIMIZE_O0
#define GBA_OPTIONS_POP
#endif
