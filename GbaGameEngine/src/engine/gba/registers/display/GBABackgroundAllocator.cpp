#include "GBABackgroundAllocator.h"
#include "gbatek/DisplayControl.h"

constexpr int MAX_BACKGROUNDS = 4;

Bitmask<u8> GetBackgroundsForCurrentVideoMode()
{
	Bitmask<u8> mask;

	switch (GBATEK::ioRegisterDisplayControl->videoMode)
	{
	case GBATEK::VideoMode::Mode0:
	{
		mask.SetBit(0);
		mask.SetBit(1);
		mask.SetBit(2);
		mask.SetBit(3);
		break;
	}
	case GBATEK::VideoMode::Mode1:
	{
		mask.SetBit(0);
		mask.SetBit(1);
		mask.SetBit(2);
		break;
	}
	case GBATEK::VideoMode::Mode2:
	{
		mask.SetBit(2);
		mask.SetBit(3);
		break;
	}
	default:
		break;
	}

	return mask;
}

namespace GBA
{
	Bitmask<u8> BackgroundAllocator::s_backgroundPoolTracker = Bitmask<u8>(0);

	int BackgroundAllocator::ReserveBackground()
	{
		int bgId = INVALID_BACKGROUND;
		Bitmask<u8> availableBackgrounds = GetBackgroundsForCurrentVideoMode();

		for (u32 i = 0; i < MAX_BACKGROUNDS; ++i)
		{
			// Test if this is already owned
			if (s_backgroundPoolTracker.TestBit(i))
				continue;

			// Test if the GBA has this background available in the first place
			if (availableBackgrounds.TestBit(i))
			{		
				bgId = i;
				break;
			}
		}

		DEBUG_ASSERTMSG(bgId < MAX_BACKGROUNDS, "Out of backgrounds, make sure appropriate backgrounds are available via DisplayControl::SetDisplayOptions");

		// Mark as reserved
		s_backgroundPoolTracker.SetBit(bgId);

		return bgId;
	}

	void BackgroundAllocator::FreeBackground(int id)
	{
		if (id < MAX_BACKGROUNDS)
		{
			s_backgroundPoolTracker.ClearBit(id);	// Clear the bit
			GBATEK::ioRegisterDisplayControl->SetBackgroundEnabled(id, false);
		}
	}
}
