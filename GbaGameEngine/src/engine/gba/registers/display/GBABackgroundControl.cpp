#include "GBABackgroundControl.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

namespace GBA
{
	BackgroundControl::tBackgrounds BackgroundControl::s_backgrounds = { Background(0), Background(1), Background(2), Background(3) };
	u8 BackgroundControl::s_backgroundPoolTracker = 0;

	Background & BackgroundControl::GetBackground(Backgrounds backgroundId)
	{
		return *s_backgrounds.At(backgroundId);
	}

	BackgroundControl::Backgrounds BackgroundControl::ReserveBackground()
	{
		BackgroundControl::Backgrounds bgId = Backgrounds::Count;

		for (u32 i = 0; i < s_backgrounds.Count(); ++i)
		{
			// Test if this is already owned
			if ((s_backgroundPoolTracker & BIT(i)) != 0)
				continue;

			DisplayOptions::ObjectRendering renderId = DisplayOptions::ObjectRendering::Background0;

			switch (i)
			{
			case 0:
			{
				renderId = DisplayOptions::ObjectRendering::Background0;
				break;
			}
			case 1:
			{
				renderId = DisplayOptions::ObjectRendering::Background1;
				break;
			}
			case 2:
			{
				renderId = DisplayOptions::ObjectRendering::Background2;
				break;
			}
			case 3:
			{
				renderId = DisplayOptions::ObjectRendering::Background3;
				break;
			}
			default:
			{
				// NOT VALID
				return Backgrounds::Count;
			}
			}

			// Test if the GBA has this background enabled in the first place
			if (DisplayControl::TestObjectRendering(renderId))
			{		
				bgId = (BackgroundControl::Backgrounds)i;
				break;
			}
		}

		DEBUG_ASSERTMSG(bgId < BackgroundControl::Backgrounds::Count, "Out of backgrounds, make sure appropriate backgrounds are available via DisplayControl::SetDisplayOptions");

		// Mark as reserved
		s_backgroundPoolTracker |= BIT(bgId);

		return bgId;
	}

	void BackgroundControl::FreeBackground(BackgroundControl::Backgrounds id)
	{
		if (id < BackgroundControl::Backgrounds::Count)
			s_backgroundPoolTracker &= ~BIT(id);	// Clear the bit
	}
}
