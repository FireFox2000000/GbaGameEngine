#include "TilemapTestScene.h"
#include "engine/engine/engine.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"

#include "engine/gba/registers/display/GBABackgroundControl.h"

TilemapTestScene::TilemapTestScene(Engine * engine) : Scene(engine)
{
}

void TilemapTestScene::Enter(Engine * engine)
{
	using namespace GBA;
	using namespace GBA::DisplayOptions;

	DisplayControl::SetDisplayOptions(Mode0 | Sprites | MappingMode1D | Background0);

	ColourPalette16 palette(0);
	tPaletteIndex paletteId = 0;

	TileBlockGroups cbb = TileBlockGroups::Bg0;
	PaletteBank::LoadBackgroundPalette(paletteId, palette);
	//Vram::AllocBackgroundMem
	//TileBank::LoadTiles(nullptr, 0, 0, cbb, 0);

	auto& background = BackgroundControl::GetBackground(BackgroundControl::Bg0);
	background.SetColourMode(Background::ColourMode::EightBitsPerPixel);
	background.SetCharacterBaseBlock(cbb);
	background.SetScreenBaseBlock(0);
	background.SetSize(Background::REG_32x32);
}

void TilemapTestScene::Update(Engine * engine)
{
}
