#include "GBABackgroundControl.h"
#include "engine/gba/registers/RegisterMap.h"

namespace GBA
{
	BackgroundControl::tBackgrounds BackgroundControl::s_backgrounds = { Background(0), Background(1), Background(2), Background(3) };

	Background & BackgroundControl::GetBackground(Backgrounds backgroundId)
	{
		return *s_backgrounds.At(backgroundId);
	}
}
