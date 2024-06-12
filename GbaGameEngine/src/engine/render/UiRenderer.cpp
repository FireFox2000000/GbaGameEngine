#include "UiRenderer.h"
#include "engine/io/filestream/CppFileReader.h"
#include "engine/gba/registers/display/GBABackgroundAllocator.h"
#include "engine/gba/graphics/tiles/GBAPaletteBank.h"
#include "engine/gba/graphics/vram/GBAVramAllocator.h"
#include "engine/gba/graphics/tilemap/GBATilemapManager.h"
#include "engine/graphics/font/FontLookupFunctions.h"
#include "engine/gba/config/GBADrawPriorityID.h"
#include "gbatek/DisplayControl.h"
#include "gbatek/Backgrounds.h"

constexpr int DynamicBackgroundSize = 32;

void DrawUiTilemap(const Vector2<int>& screenPosition, const GBA::Gfx::Tilemap* tilemap, GBA::VramAllocator::tScreenBaseBlockIndex mapSbbIndex)
{
	using namespace GBA;

	const auto* mapData = tilemap->GetTileMapData();
	const auto imageSize = tilemap->GetSizeInTiles();

	auto& vram = VramAllocator::GetInstance();

	for (int x = 0; x < imageSize.x; ++x)
	{
		for (int y = 0; y < imageSize.y; ++y)
		{
			int mapDataIndex = imageSize.x * y + x;
			int screenOffset = (screenPosition.y + y) * DynamicBackgroundSize + (screenPosition.x + x);
			vram.SetBackgroundTileData(mapSbbIndex, screenOffset, mapData[mapDataIndex]);
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

	GBATEK::ioRegisterDisplayControl->SetBackgroundEnabled(m_backgroundId, false);

	UnloadTilemapSet();

	// Free gba background
	GBA::BackgroundAllocator::FreeBackground(m_backgroundId);
	GBA::VramAllocator::GetInstance().FreeBackgroundTileMapMem(m_mapSbbIndex);
}

void UiRenderer::Init()
{
	AllocateMemory();
}

void UiRenderer::AllocateMemory()
{
	using namespace GBA;

	m_backgroundId = GBA::BackgroundAllocator::ReserveBackground();

	auto& controlRegister = (*GBATEK::ioRegisterBackgroundControls)[m_backgroundId];
	controlRegister.size = GBATEK::BackgroundSize::Regular32x32;
	controlRegister.priority = static_cast<int>(GBA::DrawPriorityID::BgUI);
	controlRegister.affineWrappingEnabled = false;
}

void UiRenderer::UnloadTilemapSet()
{
	// Palette doesn't need to be freed, auto overwritten since palette allocation isn't dynamic. 

	// Free tileset
	GBA::VramAllocator::GetInstance().FreeBackgroundTileMapMem(m_mapSbbIndex);
	GBA::VramAllocator::GetInstance().FreeBackgroundTileSetMem(m_tilemapSet.m_renderData.m_tileSetCharacterBaseBlock);
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

		// Read font properties
		m_fontProperties.fontAsciiStart = reader.Read<int>();
		m_fontProperties.firstAsciiCharacter = reader.Read<char>();
		m_fontProperties.fixedCharacterSize.x = reader.Read<int>();
		m_fontProperties.fixedCharacterSize.y = reader.Read<int>();

		// Read palette
		u8 paletteBankIndexOffset = reader.Read<u8>();
		u8 paletteLength = reader.Read<u8>();
		GBATEK::ColourRGB16* palette = reader.ReadAddress<GBATEK::ColourRGB16>(paletteLength);

		// Read tileset
		u32 compressionFlags = reader.Read<u32>();
		GBATEK::BackgroundTilemapEntry clearScreenEntry = reader.Read<GBATEK::BackgroundTilemapEntry>();

		u32 tilesetLength = reader.Read<u32>();
		GBATEK::UPixelData* tileset = reader.ReadAddress<GBATEK::UPixelData>(tilesetLength);

		// Read maps
		u8 mapCount = reader.Read<u8>();
		int tileMapDataLength = reader.Read<int>();
		u8 mapIsDynamicMask = 0;
		u8* widthMap = reader.ReadAddress<u8>(mapCount);
		u8* heightMap = reader.ReadAddress<u8>(mapCount);
		GBATEK::BackgroundTilemapEntry* mapData = reader.ReadAddress<GBATEK::BackgroundTilemapEntry>(tileMapDataLength);

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

		m_clearScreenEntry = clearScreenEntry;
	}

	// Load the palette and tileset into video memory
	{
		GBA::Gfx::TilemapManager::LoadPalette(&m_tilemapSet);
		GBA::Gfx::TilemapManager::LoadTileset(&m_tilemapSet);

		// Although we're allocating the same memory each time, call this after allocating the tileset so that our tilemap memory is located directly after the tileset
		// Otherwise we're going to eat up a whole charblock if this is called beforehand
		m_mapSbbIndex = GBA::VramAllocator::GetInstance().AllocBackgroundTileMapMem(DynamicBackgroundSize * DynamicBackgroundSize);
	}

	// Assign control register
	{
		auto& controlRegister = (*GBATEK::ioRegisterBackgroundControls)[m_backgroundId];
		controlRegister.colourMode = m_tilemapSet.m_file.m_backgroundColourMode;
		controlRegister.vramCharacterBaseBlockIndex = m_tilemapSet.GetTileSetCharacterBaseBlock();
		controlRegister.vramScreenBaseBlockIndex = m_mapSbbIndex;

		controlRegister.size = GBATEK::BackgroundSize::Regular32x32;
		controlRegister.priority = static_cast<int>(GBA::DrawPriorityID::BgUI);
		controlRegister.affineWrappingEnabled = false;

		GBATEK::ioRegisterDisplayControl->SetBackgroundEnabled(m_backgroundId, true);
	}

	// Clear the screen of any previous data or default will be first tile, need to set to the clear tile. 
	ClearRegion(0, 0, DynamicBackgroundSize, DynamicBackgroundSize);
}

void UiRenderer::DrawUiElement(const Vector2<int>& screenPositionInTiles, int uiElementIndex) const
{
	using namespace GBA::Gfx;

	const Tilemap* tilemap = m_tilemapSet.GetTilemap(uiElementIndex);
	DrawUiTilemap(screenPositionInTiles, tilemap, m_mapSbbIndex);
}

void UiRenderer::ClearRegion(int x, int y, int width, int height) const
{
	auto& vram = GBA::VramAllocator::GetInstance();

	for (int curY = 0; curY < height; ++curY)
	{
		int screenOffset = (y + curY) * DynamicBackgroundSize + x;
		vram.SetBackgroundTileData(m_mapSbbIndex, screenOffset, m_clearScreenEntry, width);
	}
}

void UiRenderer::RenderText(const std::string& str, const Vector2<int>& drawPosition) const
{
	RenderText(str.c_str(), 0, str.length(), drawPosition);
}

void UiRenderer::RenderText(const char* str, int start, int length, const Vector2<int>& drawPosition) const
{
	Vector2<int> currentDrawPosition = drawPosition;

	for (int i = start; i < (start + length); ++i)
	{
		char c = str[i];

		switch (c)
		{
		case '\n':
		{
			currentDrawPosition.x = drawPosition.x;
			currentDrawPosition.y += m_fontProperties.fixedCharacterSize.y;

			break;
		}

		default:
		{
			int uiElementIndex = StandardAsciiLookup(c) - StandardAsciiLookup(m_fontProperties.firstAsciiCharacter) + m_fontProperties.fontAsciiStart;
			DrawUiElement(currentDrawPosition, uiElementIndex);
			currentDrawPosition.x += m_fontProperties.fixedCharacterSize.x;
		}
		}
	}
}
