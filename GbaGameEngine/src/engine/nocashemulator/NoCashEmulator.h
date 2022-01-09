#pragma once
#include "external/tonc/include/tonc_nocash.h"

namespace NoCashEmulator
{
	/// <summary>
	/// Source Code Breakpoints
	/// The opcode MOV R11, R11 may be used as source code breakpoint(in both THUMB 
	/// and ARM state).These breaks are working much like normal F2 - key breakpoints,
	/// except that they may be defined directly in source code by inserting the above
	/// opcode. Because the opcode is not changing any registers or flags there will be
	/// no conflicts when testing the binary on real hardware.
	/// Note: Source breaks work only if you have enabled them in no$gba setup.To
	/// remove a specific source break, move the code - window cursor onto it, and hit
	/// the DEL - key(or enter NOP in the online assembler).
	/// Options -> Debug eXception Setup -> Source Code Breakpoints
	/// </summary>
	inline void NativeBreakpoint() {
		asm("mov r11, r11");
	}

	//!	Output a string to no$gba debugger.
	/*!
		\param str	Text to print.
		\return		Number of characters printed.
	*/
	inline void PutS(const char* str)
	{
		nocash_puts(str);
	}
}