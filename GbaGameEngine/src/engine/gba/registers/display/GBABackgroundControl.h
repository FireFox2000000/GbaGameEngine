#pragma once
#include "engine/gba/graphics/background/GBABackground.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/Bitmask.h"

namespace GBA
{
	class BackgroundControl
	{

	public:
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
