#ifndef PRAGMA_ONCE_ENGINE_GBA_REGISTERS_DISPLAY_GBADISPLAYSTATUS_H
#define PRAGMA_ONCE_ENGINE_GBA_REGISTERS_DISPLAY_GBADISPLAYSTATUS_H

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
		inline static bool InVBlank() { return IsStatusBitSet(BIT(0)); }
		inline static bool InHBlank() { return IsStatusBitSet(BIT(1)); }
		inline static vu16 VCount() { return s_REG_VCOUNT; }
	};
}

#endif
