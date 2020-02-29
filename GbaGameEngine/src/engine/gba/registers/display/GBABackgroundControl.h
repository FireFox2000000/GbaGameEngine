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
		using tBackgrounds = Array<Gfx::Background, Backgrounds::Count>;
		static tBackgrounds s_backgrounds;
		static Bitmask<u8> s_backgroundPoolTracker;

	public:
		static Gfx::Background& GetBackground(Backgrounds backgroundId);
		static Backgrounds ReserveBackground();
		static void FreeBackground(BackgroundControl::Backgrounds id);
	};
}
