#pragma once
#include "internal/Internal.h"

namespace GBATEK
{
	enum class BackgroundColourMode : unsigned char
	{
		// 4 bits-per-pixel
		Mode16x16,

		// 8 bits per pixel
		Mode256x1
	};

	enum class BackgroundSize : unsigned char
	{
		Regular32x32 = 0,
		Regular64x32 = 1,
		Regular32x64 = 2,
		Regular64x64 = 3,

		Affine16x16 = 0,
		Affine32x32 = 1,
		Affine64x64 = 2,
		Affine128x128 = 3,
	};

	struct BackgroundControl
	{
		// Lower value == higher priority
		unsigned char priority : 2;

		// 0-3
		char vramCharacterBaseBlockIndex : 2;
		char : 2; // unused
		bool mosaic : 1;
		BackgroundColourMode colourMode : 1;

		// 0-31
		unsigned char vramScreenBaseBlockIndex : 5;

		// BG2/BG3 controllers only
		bool affineWrappingEnabled : 1;

		BackgroundSize size : 2;
	};

	struct BackgroundScroll
	{
		// Write-only (0-511)
		WriteOnly<short> xOffset;
		// Write-only (0-511)
		WriteOnly<short> yOffset;
	};

	static_assert(sizeof(BackgroundControl) == 2, "BackgroundControl struct malformed");
	static_assert(sizeof(BackgroundScroll) == 4, "BackgroundScroll struct malformed");

	// Read/write
	BackgroundControl (*const ioRegisterBackgroundControls)[4] = reinterpret_cast<BackgroundControl(*const)[4]>(0x4000008);

	// Write-only
	BackgroundScroll (*const ioRegisterBackgroundScrolls)[4] = reinterpret_cast<BackgroundScroll(*const)[4]>(0x4000010);
}
