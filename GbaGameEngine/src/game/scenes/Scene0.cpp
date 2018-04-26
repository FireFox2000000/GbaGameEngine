#include "Scene0.h"
#include "engine\base\colour\Colour.h"
#include "engine\gba\registers\display\GBADisplayControl.h"
#include "engine\gba\graphics\tiles\GBAPaletteBank.h"
#include "engine\gba\graphics\tiles\GBATileBank.h"
#include "engine\rendering\GBASpriteRenderer.h"
#include "game\blobdata\Shantae.h"

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
	for (u32 i = 0; i < Shantae::paletteLength; ++i)
	{
		shantaePalette[i] = Shantae::palette[i];
	}

	tPaletteBlockId testPaletteId = 0;
	PaletteBank::LoadSpritePalette(testPaletteId, shantaePalette);

	tSpriteData tileData0;
	tileData0.Reserve(Shantae::dataLength);
	for (u32 i = 0; i < Shantae::dataLength; ++i)
	{
		tileData0.Add(Shantae::data[i]);
	}

	tTileId tileIndex = 4;
	TileBank::LoadSpriteTiles(tileData0, tileIndex);

	testSprite.Init(Attributes::Tall, Attributes::Form3, testPaletteId, tileIndex);
	testRenderer->SetSprite(&testSprite);
}

Scene0::~Scene0()
{
}
