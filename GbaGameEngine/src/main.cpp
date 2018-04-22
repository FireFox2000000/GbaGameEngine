#include "engine\assets\Sprite.h"
#include "engine\base\colour\Colour.h"
#include "engine\base\core\stl\List.h"
#include "engine\gba\input\GBAInput.h"
#include "engine\gba\graphics\displayregisters\GBADisplayControl.h"
#include "engine\gba\graphics\displayregisters\GBADisplayStatus.h"
#include "engine\gba\graphics\tiles\GBAPaletteBank.h"
#include "engine\gba\graphics\tiles\GBATileBank.h"
#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\math\Vector2.h"
#include "engine\gameobject\GameObject.h"
#include "engine\rendering\SpriteRenderer.h"

#define VBLANK_SCNLNE_START 160

static void WaitForVSync();

int main()
{
	using namespace GBA;

	List<GameObject> gameObjects;
	List<SpriteRenderer> renderList;

	GameObject* testObject = gameObjects.AddNew();
	SpriteRenderer* testRenderer = renderList.Add(SpriteRenderer(testObject));
	Sprite testSprite;
	
	// Test Initialisation
	{		
		using namespace DisplayOptions;	
		DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);
		Input::Update();

		ColourPalette16 colourPalette0(0);
		colourPalette0[0] = 0;	// Transparent
		colourPalette0[1] = Colour::White.RGB16();
		colourPalette0[2] = Colour::Red.RGB16();
		colourPalette0[3] = Colour::Blue.RGB16();
		
		tPaletteBlockId testPaletteId = 0;
		PaletteBank::LoadSpritePalette(testPaletteId, colourPalette0);

		tSpriteData tileData0;
		for (int i = 0; i < 64; ++i)		// Fills 4 tiles, 64 / 16
		{
			tileData0.Add(0x3212);
		}

		tTileId tileIndex = 4;
		TileBank::LoadSpriteTiles(tileData0, tileIndex);

		testSprite.Init(Attributes::Wide, Attributes::Form1, testPaletteId, tileIndex);
		testRenderer->SetSprite(&testSprite);
	}

	// Update loop
	while (1)
	{
		// General update
		Input::Update();

		for (List<GameObject>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			it->Update();
		}

		// Main update
		WaitForVSync();

		// Render
		for (u32 i = 0; i < renderList.Count(); ++i)
		{
			renderList[i].Render();
		}
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
