#pragma once

namespace GBA
{
	enum class BackgroundDrawPriority
	{
		Layer0,	// Highest
		Layer1,
		Layer2,
		Layer3	// Lowest
	};

	enum class CharacterBaseBlock
	{
		Bg0,
		Bg1,
		Bg2,
		Bg3,
	};

	enum class ColourMode
	{
		Mode16x16,
		Mode256x1
	};

	enum class BackgroundSize
	{
		REG_32x32 = 0,
		REG_64x32 = 1,
		REG_32x64 = 2,
		REG_64x64 = 3,
		REGSize_Count = 4,

		AFF_16x16 = 0,
		AFF_32x32 = 1,
		AFF_64x64 = 2,
		AFF_128x128 = 3,
		AFFSize_Count = 4,
	};

	struct BackgroundControl
	{
		BackgroundDrawPriority priority : 2;
		CharacterBaseBlock characterBaseBlock : 2;
		char : 2; // unused
		bool mosaic : 1;
		ColourMode colourMode : 1;
		unsigned short screenBaseBlock : 5;

		// BG2/BG3 controllers only
		bool affineWrappingEnabled : 1;

		BackgroundSize size : 2;
	};

	struct BackgroundScroll
	{
	private:
		// (0-511)
		short xOffset; //  : 9, : 7 unused;
		short yOffset; //  : 9, : 7 unused;

	public:
		inline void SetXOffset(short val) { xOffset = val; }
		inline void SetYOffset(short val) { yOffset = val; }
	};

	// Read/write
	BackgroundControl (*const ioRegisterBackgroundControls)[4] = reinterpret_cast<BackgroundControl(*const)[4]>(0x4000008);

	// Write-only
	BackgroundScroll (*const ioRegisterBackgroundScrolls)[4] = reinterpret_cast<BackgroundScroll(*const)[4]>(0x4000010);
}
