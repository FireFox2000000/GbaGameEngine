#pragma once
#include "engine/gba/graphics/background/GBABackground.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/Bitmask.h"

namespace GBA
{
	// Provides access to the physical background controls registers of the GBA.
	class BackgroundControl
	{

	public:
		/// <summary>
		/// Available background on the GBA. Changes upon which display mode is set. 
		/// Note that Bg0 renders on top of Bg1, which is on top of Bg2 etc. 
		/// </summary>
		enum Backgrounds
		{
			Bg0,
			Bg1,
			Bg2,
			Bg3,

			Count
		};

	private:
		static Bitmask<u8> s_backgroundPoolTracker;

	public:
		static Gfx::Background::ControlRegister& GetBgControlRegister(Backgrounds backgroundId);
		static void SetBackgroundScrollingPosition(Backgrounds backgroundId, u16 x, u16 y); // Position of the screen on the map, write-only

		static Backgrounds ReserveBackground();
		static void FreeBackground(BackgroundControl::Backgrounds id);
	};
}
