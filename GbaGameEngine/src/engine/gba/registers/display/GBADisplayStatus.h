#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

// http://www.coranac.com/tonc/text/video.htm

namespace GBA
{
	class DisplayStatus
	{
		static vu32& s_REG_DISPSTAT;
		static const vu16& s_REG_VCOUNT;

		inline static bool IsStatusBitSet(int bitIndex) { return s_REG_DISPSTAT & bitIndex; }
	public:

		enum Flags
		{
			VBlankStatus = BIT(0),
			HBlankStatus = BIT(1),
			VCountTriggerStatus = BIT(2),	// Set if the current scanline matches the scanline trigger ( REG_VCOUNT == REG_DISPSTAT{8-F} )
			VBlankInterruptRequest = BIT(3),
			HBlankInterruptRequest = BIT(4),
			VCountInterruptRequest = BIT(15), // Fires interrupt if current scanline matches trigger value.
		};

		inline static bool InVBlank() { return IsStatusBitSet(VBlankStatus); }
		inline static bool InHBlank() { return IsStatusBitSet(HBlankStatus); }
		inline static vu16 VCount() { return s_REG_VCOUNT; }
	};
}
