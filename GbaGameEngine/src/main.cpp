#include "engine\math\Vector2.h"
#include "engine\gba\input\GBAInput.h"
#include "engine\gba\sprites\GBAObjectAttribute.h"
#include "engine\gba\graphics\displayregisters\GBADisplayControl.h"
#include "engine\gba\graphics\displayregisters\GBADisplayStatus.h"
#include "engine\base\colour\Colour.h"

#define VBLANK_SCNLNE_START 160

static void WaitForVSync();

int main()
{
	using namespace GBA;

	// Initialisation
	{		
		using namespace DisplayOptions;	
		DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);
		Input::Update();
	}

	// Update loop
	while (1)
	{
		// General update
		Input::Update();

		// Main update
		WaitForVSync();

		// Render

	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}