#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"

#include "engine/gba/registers/display/GBABackgroundControl.h"

#include "engine/gba/registers/input/GBAInput.h"

namespace __binary_background_Shantae_Idle_Map
{
	extern const u8 paletteLength;
	extern const u32 tilesetLength;

	extern const u16 palette[];
	extern const u32 tileset[];

	extern const u16 mapLength;
	extern const u16 map[];
}

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;
	using namespace __binary_background_Shantae_Idle_Map;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D | Background0);

	ColourPalette256 colourPalette(0);

	for (u32 i = 0; i < paletteLength; ++i)
	{
		colourPalette[i] = palette[i];
	}

	TileBlockGroups cbb = TileBlockGroups::Bg0;
	tScreenBaseBlockIndex sbb = 0;
	PaletteBank::LoadBackgroundPalette(colourPalette);
	Vram::GetInstance().AllocBackgroundMem(tileset, tilesetLength, map, mapLength, cbb, sbb);

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
