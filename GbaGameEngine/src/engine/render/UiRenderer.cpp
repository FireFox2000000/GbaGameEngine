#include "UiRenderer.h"
#include "engine/filestream/CppFileReader.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVram.h"
#include "engine/gba/registers/display/GBADisplayControl.h"
#include "engine/gba/graphics/tilemap/GBATilemapManager.h"

constexpr int BackgroundSize = 32;

void DrawUiTilemap(const Vector2<int>& screenPosition, const GBA::Gfx::Tilemap* tilemap, GBA::tScreenBaseBlockIndex mapSbbIndex)
{
	using namespace GBA;

	const u16* mapData = tilemap->GetTileMapData();
	const auto imageSize = tilemap->GetSizeInTiles();

	for (int x = 0; x < imageSize.x; ++x)
	{
		for (int y = 0; y < imageSize.y; ++y)
		{
			int mapDataIndex = imageSize.x * y + x;
			int screenOffset = (screenPosition.y + y) * BackgroundSize + (screenPosition.x + x);
			Vram::GetInstance().SetBackgroundTileData(mapSbbIndex, screenOffset, mapData[mapDataIndex]);
		}
	}
}

UiRenderer::UiRenderer()
{
	Init();
}

UiRenderer::~UiRenderer()
{
	DEBUG_LOG("Unloading UiRenderer");

	GBA::DisplayControl::SetBackgroundActive(m_backgroundId, false);

	UnloadTilemapSet();

	// Free gba background
	GBA::BackgroundControl::FreeBackground(m_backgroundId);
	GBA::Vram::GetInstance().FreeBackgroundTileMapMem(m_mapSbbIndex);
}

void UiRenderer::Init()
{
	AllocateMemory();
}

void UiRenderer::AllocateMemory()
{
	using namespace GBA;

	m_backgroundId = GBA::BackgroundControl::ReserveBackground();

	auto& controlRegister = BackgroundControl::GetBgControlRegister(m_backgroundId);
	controlRegister.SetSize(Gfx::Background::ControlRegister::REG_32x32);
	controlRegister.SetPriority(GBA::Gfx::DrawPriority::Layer0);
	controlRegister.SetAffineWrapping(false);
}

void UiRenderer::UnloadTilemapSet()
{
	// Palette doesn't need to be freed, auto overwritten since palette allocation isn't dynamic. 

	// Free tileset
	GBA::Vram::GetInstance().FreeBackgroundTileMapMem(m_mapSbbIndex);
	GBA::Vram::GetInstance().FreeBackgroundTileSetMem(m_tilemapSet.m_renderData.m_tileSetCharacterBaseBlock);
}

void UiRenderer::LoadAtlus(const u32* file)
{
	DEBUG_LOG("Loading UiAtlus into UiRenderer");

	using namespace GBA;
	using namespace GBA::Gfx;

	// Unload old tilemap if we already have one loaded.
	UnloadTilemapSet();

	// Read tilemapset from file
	{
		CppFileReader reader = CppFileReader(file);

		// Read palette
		u8 paletteBankIndexOffset = reader.Read<u8>();
		u8 paletteLength = reader.Read<u8>();
		u16* palette = reader.ReadAddress<u16>(paletteLength);

		// Read tileset
		u32 compressionFlags = reader.Read<u32>();

		u32 tilesetLength = reader.Read<u32>();
		DEBUG_LOGFORMAT("Ui atlus tileset length = %d", tilesetLength);
		u32* tileset = reader.ReadAddress<u32>(tilesetLength);

		// Read maps
		u8 mapCount = reader.Read<u8>();
		int tileMapDataLength = reader.Read<int>();
		u8 mapIsDynamicMask = 0;
		u8* widthMap = reader.ReadAddress<u8>(mapCount);
		u8* heightMap = reader.ReadAddress<u8>(mapCount);
		u16* mapData = reader.ReadAddress<u16>(tileMapDataLength);

		m_tilemapSet = TilemapSet(
			paletteBankIndexOffset
			, paletteLength
			, palette
			, tilesetLength
			, tileset
			, compressionFlags
			, mapCount
			, mapIsDynamicMask
			, widthMap
			, heightMap
			, mapData
		);
	}

	// Load the palette and tileset into video memory
	{
		GBA::Gfx::TilemapManager::LoadPalette(&m_tilemapSet);
		GBA::Gfx::TilemapManager::LoadTileset(&m_tilemapSet);
	}

	// Assign control register
	{
		m_mapSbbIndex = GBA::Vram::GetInstance().AllocBackgroundTileMapMem(BackgroundSize * BackgroundSize);	// Must call this after AllocBackgroundTileSetMem. May be a bug though.

		GBA::Gfx::Background::ControlRegister::ColourMode colourMode = GBA::Gfx::Background::GetColourModeFromCompression(m_tilemapSet.m_file.m_tileSetDataCompressionFlags);
		auto& controlRegister = BackgroundControl::GetBgControlRegister(m_backgroundId);
		controlRegister.SetColourMode(colourMode);
		controlRegister.SetCharacterBaseBlock(m_tilemapSet.GetTileSetCharacterBaseBlock());
		controlRegister.SetScreenBaseBlock(m_mapSbbIndex);

		controlRegister.SetSize(Gfx::Background::ControlRegister::REG_32x32);
		controlRegister.SetPriority(GBA::Gfx::DrawPriority::Layer0);
		controlRegister.SetAffineWrapping(false);

		GBA::DisplayControl::SetBackgroundActive(m_backgroundId, true);
	}
}

void UiRenderer::DrawUiElement(const Vector2<int>& screenPositionInTiles, int uiElementIndex)
{
	using namespace GBA::Gfx;

	Tilemap* tilemap = m_tilemapSet.GetTilemap(uiElementIndex);
	DrawUiTilemap(screenPositionInTiles, tilemap, m_mapSbbIndex);
}
