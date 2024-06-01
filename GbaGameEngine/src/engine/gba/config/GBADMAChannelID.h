#pragma once
#include "engine/base/Typedefs.h"

namespace GBA
{
	// Total of channels 0-3, add to the list to reserve channels for the engine.
	enum class DMAChannelID
	{
		Sound0 = 1,
		Sound1 = 2,
		SoundCount = 2,

		Invalid = -1
	};
}