#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"

#include "engine/gba/registers/display/GBABackgroundControl.h"

#include "game/data/brin.h"
#include "engine/gba/registers/input/GBAInput.h"

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D | Background0);

	ColourPalette256 palette(0);

	for (u32 i = 0; i < 256; ++i)
	{
		palette[i] = brinPalette[i];
	}

	//tPaletteIndex paletteId = 3;

	TileBlockGroups cbb = TileBlockGroups::Bg0;
	tScreenBaseBlockIndex sbb = 0;
	PaletteBank::LoadBackgroundPalette(palette);
	Vram::GetInstance().AllocBackgroundMem(brinTiles, 584, brinMap, 1024, cbb, sbb);

	auto& background = BackgroundControl::GetBackground(BackgroundControl::Bg0);
	background.SetColourMode(Background::ColourMode::FourBitsPerPixel);
	background.SetCharacterBaseBlock(cbb);
	background.SetScreenBaseBlock(sbb);
	background.SetSize(Background::REG_32x32);
}

void TilemapTestScene::Update(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	if (GBA::Input::GetKey(GBA::Buttons::Left))
	{
		bgPosition.x += -1;
	}
	
	if (GBA::Input::GetKey(GBA::Buttons::Right))
	{
		bgPosition.x += 1;
	}

	if (GBA::Input::GetKey(GBA::Buttons::Down))
	{
		bgPosition.y += 1;
	}

	if (GBA::Input::GetKey(GBA::Buttons::Up))
	{
		bgPosition.y += -1;
	}

	auto& background = BackgroundControl::GetBackground(BackgroundControl::Bg0);
	background.SetPosition(bgPosition);
}
