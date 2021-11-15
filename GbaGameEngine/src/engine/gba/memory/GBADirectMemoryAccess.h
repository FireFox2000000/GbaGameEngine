#pragma once
#include "engine/base/Typedefs.h"

namespace GBA
{
	namespace DirectMemoryAccess
	{
		// https://www.coranac.com/tonc/text/dma.htm
		enum Channels
		{
			Channel0,		// Highest priority, only useable with internal ram
			Channel1,
			Channel2,
			Channel3,		// Lowest priority, use for general purpose copies

			Count,

			Sound0 = Channel1,
			Sound1 = Channel2,
			General = Channel3,
		};

		namespace Mode
		{
#define DesinationAdjustmentBitOffset 0x15
#define SourceAdjustmentBitOffset 0x17
#define ChunkSizeBitOffset 0x1A
#define TimingModeBitOffset 0x1C

			// TODO, bit set these values
			namespace DesinationAdjustment
			{
				enum Enum
				{
					Increment = 0 << DesinationAdjustmentBitOffset,
					Decrement = 1 << DesinationAdjustmentBitOffset,
					Fixed = 2 << DesinationAdjustmentBitOffset,		// address is fixed
					Reload = 3 << DesinationAdjustmentBitOffset,		// increment the destination during the transfer, and reset it so that repeat DMA will always start at the same destination.
				};
			}

			namespace SourceAdjustment
			{
				enum Enum
				{
					Increment = 0 << SourceAdjustmentBitOffset,
					Decrement = 1 << SourceAdjustmentBitOffset,
					Fixed = 2 << SourceAdjustmentBitOffset,		// address is fixed
				};
			}

			enum TimingMode
			{
				Immediate = 0 << TimingModeBitOffset,
				AtVBlank = 1 << TimingModeBitOffset,
				AtHBlank = 2 << TimingModeBitOffset,
				Special = 3 << TimingModeBitOffset,	// The 'Special' setting (Start Timing=3) depends on the DMA channel: DMA0 = Prohibited, DMA1 / DMA2 = Sound FIFO, DMA3 = Video Capture
			};

			enum ChunkSize
			{
				Size16 = 0 << ChunkSizeBitOffset,
				Size32 = 1 << ChunkSizeBitOffset,
			};

			enum Flags
			{
				Repeat = 1 << 0x19,				// Repeats the copy at each VBlank or HBlank if the DMA timing has been set to those modes. Must be 0 if timing mode is set.
				InterruptRequest = 1 << 0x1E,	// Raise an interrupt when finished.
				Enable = 1 << 0x1F,				// Enable the DMA transfer for this channel. Maybe just a tad important.
			};

#undef DesinationAdjustmentBitOffset
#undef SourceAdjustmentBitOffset
#undef ChunkSizeBitOffset
		}

		// After changing the Enable bit from 0 to 1, wait 2 clock cycles before accessing any DMA related registers.
		void Transfer(Channels channel, volatile void *dst, const void *src, u32 count, u32 mode);
		void Reset(Channels channel);
	}
}