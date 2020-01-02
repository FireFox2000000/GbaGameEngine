#pragma once
#include "engine/base/core/stl/Array.h"
#include "GBABackground.h"

namespace GBA
{
	class BgVramAllocator
	{
		// Character block = tilesets
		// Screen entry = individual tile map data
		// Allocation in terms of screen entries. 
		// Total of 4 character blocks, i.e. where tilesets start from
		// 8 screenblocks per character block for a total of 32
		// Each SE is 2048 bytes

		static const u32 MAX_SCREEN_ENTRIES = 32;

		enum SEAllocationState
		{
			Free,
			Used,
			Continue
		};

		Array<SEAllocationState, MAX_SCREEN_ENTRIES> m_screenEntryTracker;

	public:
		BgVramAllocator();

		void Alloc(u32 totalBytes, Background::tCharacterBaseBlock& out_cbbIndex, Background::tScreenBaseBlockIndex& out_sbbIndex);
		void Free(Background::tScreenBaseBlockIndex sbbIndex);
		void Clear();
	};
}
