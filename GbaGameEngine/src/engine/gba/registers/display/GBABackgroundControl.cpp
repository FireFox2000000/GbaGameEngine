#include "GBABackgroundControl.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

namespace GBA
{
	BackgroundControl::tBackgrounds BackgroundControl::s_backgrounds = { Background(0), Background(1), Background(2), Background(3) };
	Bitmask<u8> BackgroundControl::s_backgroundPoolTracker = Bitmask<u8>(0);

	Background & BackgroundControl::GetBackground(Backgrounds backgroundId)
	{
		return *s_backgrounds.At(backgroundId);
	}

	BackgroundControl::Backgrounds BackgroundControl::ReserveBackground()
	{
		BackgroundControl::Backgrounds bgId = Backgrounds::Count;
		Bitmask<u8> availableBackgrounds = DisplayControl::GetBackgroundsForCurrentVideoMode();

		for (u32 i = 0; i < s_backgrounds.Count(); ++i)
		{
			// Test if this is already owned
			if (s_backgroundPoolTracker.TestBit(i))
				continue;

			// Test if the GBA has this background available in the first place
			if (availableBackgrounds.TestBit(i))
			{		
				bgId = (BackgroundControl::Backgrounds)i;
				break;
			}
		}

		DEBUG_ASSERTMSG(bgId < BackgroundControl::Backgrounds::Count, "Out of backgrounds, make sure appropriate backgrounds are available via DisplayControl::SetDisplayOptions");

		// Mark as reserved
		s_backgroundPoolTracker.SetBit(bgId);
		DisplayControl::SetBackgroundActive(bgId, true);

		return bgId;
	}

	void BackgroundControl::FreeBackground(BackgroundControl::Backgrounds id)
	{
		if (id < BackgroundControl::Backgrounds::Count)
		{
			s_backgroundPoolTracker.ClearBit(id);	// Clear the bit
			DisplayControl::SetBackgroundActive(id, false);
		}
	}
}
