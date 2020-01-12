#pragma once
#include "engine/gba/graphics/background/GBABackground.h"
#include "engine/base/core/stl/Array.h"

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
		using tBackgrounds = Array<Background, Backgrounds::Count>;
		static tBackgrounds s_backgrounds;
		static u8 s_backgroundPoolTracker;

	public:
		static Background& GetBackground(Backgrounds backgroundId);
		static Backgrounds ReserveBackground();
		static void FreeBackground(BackgroundControl::Backgrounds id);
	};
}
