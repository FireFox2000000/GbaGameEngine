#pragma once

namespace GBA
{
	enum class DMADesinationAdjustment : unsigned short
	{
		Increment,
		Decrement,
		Fixed,
		Reload,		// increment the destination during the transfer, and reset it so that repeat DMA will always start at the same destination.
	};

	enum class DMASourceAdjustment : unsigned short
	{
		Increment,
		Decrement,
		Fixed,
	};

	enum class DMARepeat : unsigned short
	{
		Off,
		On
	};

	enum class DMATransferType : unsigned short
	{
		CopyBy16Bits,	// Halfword
		CopyBy32Bits	// Word
	};

	enum class DMATimingMode : unsigned short
	{
		Immediate,
		AtVBlank,
		AtHBlank,

		// The 'Special' setting (Start Timing=3) depends on the DMA channel: 
		// DMA0 = Prohibited
		// DMA1 / DMA2 = Sound FIFO
		// DMA3 = Video Capture
		Special,	
	};

	enum class DMAInterrupt : unsigned short
	{
		Disabled,
		Enabled
	};

	enum class DMAEnabled : unsigned short
	{
		Off,
		On
	};

	// A value of zero is treated as max length (ie. 0x4000, or 0x10000 for DMA3)
	static constexpr int DMA_UNIT_TRANSFER_COUNT_MAX = 0;

	struct DMARegister
	{
		const void* src;
		volatile void* dst;

		// Number of units to transfer
		// Units are either 16 or 32 bits, based on DMATransferType
		// A value of zero is treated as max length (ie. 0x4000, or 0x10000 for DMA3)
		unsigned short unitTransferCount;

		struct Control
		{
			unsigned short : 5;
			DMADesinationAdjustment destinationAdjustment : 2;
			DMASourceAdjustment sourceAdjustment : 2;
			
			// Repeats the copy at each VBlank or HBlank if the DMA timing has been set to those modes. 
			// Must be 0 if Game Pak DRQ bit is set.
			DMARepeat repeat : 1;
			DMATransferType transferType : 1;
			unsigned short : 1;	// TODO - Game Pak DRQ  - DMA3 only -  (0=Normal, 1=DRQ <from> Game Pak, DMA3)
			DMATimingMode timingMode : 2;
			DMAInterrupt raiseInterruptUponCompletion : 1;

			// This flag is automatically cleared upon completion of the transfer. 
			// The user may also clear this bit manually in order to stop the transfer
			DMAEnabled enabled : 1;
		} control;
	};

	static_assert(sizeof(DMARegister) == 12, "DMARegister struct malformed");

	volatile DMARegister(* const ioRegisterDMA)[4] = reinterpret_cast<volatile DMARegister(* const)[4]>(0x40000B0);
}
