#include "engine\assets\GBASprite.h"
#include "engine\base\colour\Colour.h"
#include "engine\base\core\stl\List.h"
#include "engine\gba\registers\input\GBAInput.h"
#include "engine\gba\registers\display\GBADisplayControl.h"
#include "engine\gba\registers\display\GBADisplayStatus.h"
#include "engine\gba\graphics\tiles\GBAPaletteBank.h"
#include "engine\gba\graphics\tiles\GBATileBank.h"
#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\math\Vector2.h"
#include "engine\gameobject\GameObject.h"
#include "engine\gameobject\Camera.h"
#include "engine\rendering\GBASpriteRenderer.h"

#define VBLANK_SCNLNE_START 160

static void WaitForVSync();

int main()
{
	using namespace GBA;

	Camera mainCamera;
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

		ColourPalette16 shantaePalette(0);
		shantaePalette[0] = 0;	// Transparent
		shantaePalette[1] = Colour(160, 0, 216).RGB16();	// Shantae purple
		shantaePalette[2] = Colour(240, 240, 0).RGB16();
		shantaePalette[3] = Colour(240, 192, 128).RGB16();
		shantaePalette[4] = Colour(176, 240, 240).RGB16();
		shantaePalette[5] = Colour(240, 0, 0).RGB16();
		
		tPaletteBlockId testPaletteId = 0;
		PaletteBank::LoadSpritePalette(testPaletteId, shantaePalette);

		tSpriteData tileData0;
		for (int i = 0; i < 64 / 8; ++i)		// Fills 4 tiles, 64 / 16
		{
			tileData0.Add(0x1111);
			tileData0.Add(0x1111);
			tileData0.Add(0x2222);
			tileData0.Add(0x2222);
			tileData0.Add(0x3333);
			tileData0.Add(0x3333);
			tileData0.Add(0x4444);
			tileData0.Add(0x4444);
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
			renderList[i].Render(&mainCamera);
		}
	}

	return 0;
}

static void WaitForVSync()
{
	while (GBA::DisplayStatus::VCount() >= VBLANK_SCNLNE_START);   // wait till VDraw
	while (GBA::DisplayStatus::VCount() < VBLANK_SCNLNE_START);    // wait till VBlank
}
