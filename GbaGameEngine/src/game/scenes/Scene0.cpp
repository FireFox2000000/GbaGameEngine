#include "Scene0.h"
#include "engine\base\colour\Colour.h"
#include "engine\gba\registers\display\GBADisplayControl.h"
#include "engine\gba\graphics\tiles\GBAPaletteBank.h"
#include "engine\gba\graphics\tiles\GBATileBank.h"
#include "engine\rendering\GBASpriteRenderer.h"

#include "game\scripts\MovementTest.h"

Scene0::Scene0()
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	GameObject* testObject = gameObjects.AddNew();
	testObject->AddComponent<MovementTest>();
	SpriteRenderer* testRenderer = testObject->AddComponent<SpriteRenderer>();
	renderList.Add(testRenderer);

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D);

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

Scene0::~Scene0()
{
}
