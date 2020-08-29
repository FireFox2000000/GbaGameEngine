#include "GBABackgroundControl.h"
#include "engine/gba/registers/RegisterMap.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

namespace GBA
{
	using tBgControlRegisters = Array<GBA::Gfx::Background::ControlRegister, BackgroundControl::Backgrounds::Count>;
	static tBgControlRegisters& s_backgroundControlRegisters = *reinterpret_cast<tBgControlRegisters*>(REG_BGCNT);

	using tBgScrollingRegisters = Array<GBA::Gfx::Background::Position, BackgroundControl::Backgrounds::Count>;
	static tBgScrollingRegisters& s_backgroundScrollingRegisters = *reinterpret_cast<tBgScrollingRegisters*>(&s_backgroundControlRegisters[0] + s_backgroundControlRegisters.Count());

	Bitmask<u8> BackgroundControl::s_backgroundPoolTracker = Bitmask<u8>(0);

	Gfx::Background::ControlRegister & BackgroundControl::GetBgControlRegister(Backgrounds backgroundId)
	{
		return *s_backgroundControlRegisters.At(backgroundId);
	}

	void BackgroundControl::SetBackgroundScrollingPosition(Backgrounds backgroundId, u16 x, u16 y)
	{
		GBA::Gfx::Background::Position& bgPos = *s_backgroundScrollingRegisters.At(backgroundId);
		bgPos.SetX(x);
		bgPos.SetY(y);
	}

	BackgroundControl::Backgrounds BackgroundControl::ReserveBackground()
	{
		BackgroundControl::Backgrounds bgId = Backgrounds::Count;
		Bitmask<u8> availableBackgrounds = DisplayControl::GetBackgroundsForCurrentVideoMode();

		for (u32 i = 0; i < s_backgroundControlRegisters.Count(); ++i)
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
