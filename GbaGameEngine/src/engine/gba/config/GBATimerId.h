#pragma once
#include "engine/base/Typedefs.h"

// Configureable based on use-case
enum GBATimerId : u8
{
	Sound0,
#ifdef PROFILE
	Profile,
#else
	Sound1,
#endif
	SystemClock1,
	SystemClock2,

	
};
