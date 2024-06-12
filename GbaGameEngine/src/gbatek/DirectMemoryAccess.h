#pragma once

namespace GBATEK
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

	enum class DMATimingModeDefault : unsigned short
	{
		Immediate,
		AtVBlank,
		AtHBlank,
	};

	enum class DMATimingModeSound : unsigned short
	{
		Immediate,
		AtVBlank,
		AtHBlank,
		SoundFIFO
	};

	enum class DMATimingModeVideo : unsigned short
	{
		Immediate,
		AtVBlank,
		AtHBlank,

		// Intended to copy a bitmap from memory (or from external hardware/camera) to VRAM. 
		// When using this transfer mode, set the repeat bit, and write the number of data units 
		// (per scanline) to the word count register. 
		// Capture works similar like HBlank DMA, however, the transfer is started when 
		// VCOUNT=2, it is then repeated each scanline, and it gets stopped when VCOUNT=162.
		VideoCapture
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

	enum class DMAGamePakDRQ : unsigned short
	{
		Off,
		On
	};

	// A value of zero is treated as max length (ie. 0x4000, or 0x10000 for DMA3)
	static constexpr int DMA_UNIT_TRANSFER_COUNT_MAX = 0;

	struct DMARegister0
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
			unsigned short : 1;
			DMATimingModeDefault timingMode : 2;
			DMAInterrupt raiseInterruptUponCompletion : 1;

			// This flag is automatically cleared upon completion of the transfer. 
			// The user may also clear this bit manually in order to stop the transfer
			DMAEnabled enabled : 1;
		} control;
	};

	struct DMARegister1And2
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
			unsigned short : 1;
			DMATimingModeSound timingMode : 2;
			DMAInterrupt raiseInterruptUponCompletion : 1;

			// This flag is automatically cleared upon completion of the transfer. 
			// The user may also clear this bit manually in order to stop the transfer
			DMAEnabled enabled : 1;
		} control;
	};

	struct DMARegister3
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

			// When setting the 'Game Pack DRQ' bit, then the cartridge must contain an 
			// external circuit which outputs a /DREQ signal. 
			// Note that there is only one pin for /DREQ and /IREQ, thus the cartridge 
			// may not supply /IREQs while using DRQ mode.
			DMAGamePakDRQ gamePakDrqEnabled : 1;
			DMATimingModeVideo timingMode : 2;
			DMAInterrupt raiseInterruptUponCompletion : 1;

			// This flag is automatically cleared upon completion of the transfer. 
			// The user may also clear this bit manually in order to stop the transfer
			DMAEnabled enabled : 1;
		} control;
	};

	static_assert(sizeof(DMARegister0) == 12, "DMARegister0 struct malformed");
	static_assert(sizeof(DMARegister1And2) == 12, "DMARegister1And2 struct malformed");
	static_assert(sizeof(DMARegister3) == 12, "DMARegister3 struct malformed");

	volatile DMARegister0* const ioRegisterDMA0 = reinterpret_cast<volatile DMARegister0* const>(0x40000B0);
	volatile DMARegister1And2* const ioRegisterDMA1 = reinterpret_cast<volatile DMARegister1And2* const>(0x40000BC);
	volatile DMARegister1And2* const ioRegisterDMA2 = reinterpret_cast<volatile DMARegister1And2* const>(0x40000C8);
	volatile DMARegister3* const ioRegisterDMA3 = reinterpret_cast<volatile DMARegister3* const>(0x40000D4);
}
