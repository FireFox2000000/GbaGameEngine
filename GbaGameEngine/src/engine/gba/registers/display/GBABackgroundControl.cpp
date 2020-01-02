#include "GBABackgroundControl.h"
#include "engine/gba/registers/RegisterMap.h"

namespace GBA
{
	BackgroundControl::tBackgrounds* BackgroundControl::s_backgrounds = reinterpret_cast<BackgroundControl::tBackgrounds*>(REG_BGCNT);

	Background & BackgroundControl::GetBackground(Backgrounds backgroundId)
	{
		return *s_backgrounds->At(backgroundId);
	}
}
