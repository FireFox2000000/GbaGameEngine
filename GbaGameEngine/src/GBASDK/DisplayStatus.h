#pragma once
namespace GBA
{
	struct DisplayStatus
	{
		const bool isInVBlank : 1;
		const bool isInHBlank : 1;
		const bool isInVCount : 1;

		bool vBlankInterruptRequestEnabled : 1;
		bool hBlankInterruptRequestEnabled : 1;
		bool vCountInterruptRequestEnabled : 1;
		unsigned short : 2;	// bits 6-7 unused

		// VCount trigger value. If the current scanline is at this value, bit 2 is set and an interrupt is fired if requested.
		// (0..227)
		unsigned short vCountTrigger : 8;		
	};

	// Read/write
	volatile DisplayStatus* const ioRegisterDisplayStatus = reinterpret_cast<DisplayStatus*>(0x4000004);
}