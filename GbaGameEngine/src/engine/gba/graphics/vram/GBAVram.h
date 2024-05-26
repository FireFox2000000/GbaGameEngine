#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/tiles/GBATile.h"
#include "GBASDK/Vram.h"

namespace GBA
{
	struct BackgroundTilemapEntry;
	union UPixelData;

	// Charblocks/Character base block: region of memory for tilesets to be placed. 
	// ScreenBlocks: region of memory for screen entires to be placed.

	enum TileBlockGroups
	{
		Bg0,
		Bg1,
		Bg2,
		Bg3,
		SpriteLower,		// May be used by backgrounds in bitmap modes
		SpriteHigher,		// This is the only available sprite vram in bitmap modes

		BlockGroupCount
	};

	using tScreenBaseBlockIndex = u16;
	const tScreenBaseBlockIndex INVALID_SBB_ID = -1;

	typedef List<u32> tSpriteData;

	class Vram
	{
	private:
		bool LoadTiles(const GBA::UPixelData* pixelMap, u32 pixelMapSize, u32 compressionFlags, TileBlockGroups tileBlockGroup, u16 startTileIndex);

		enum AllocState
		{
			Free,
			Used,
			Continue
		};

		// Sprite tile mem allocator
		// See "OBJ Sprite Memory Management" - https://www.gamasutra.com/view/feature/131491/gameboy_advance_resource_management.php?print=1
		// This will be wrong in bitmap modes, will only have half memory
		static constexpr u32 MAX_SPRITE_TILES = ARRAY_SIZE(GBA::VramTileMode::objectTiles);
		Array<AllocState, MAX_SPRITE_TILES> m_spriteTileMemTracker;
		GBA::Gfx::tTileId FindNextFreeSpriteTileSpace(u8 tileCount) const;

		// Background tile mem allocator
		static constexpr u32 MaxScreenBlocks = ARRAY_SIZE(GBA::VramTileMode::UBackgroundMapsAndTiles::screenBaseBlocks);
		Array<AllocState, MaxScreenBlocks> m_screenEntryTracker;

		Vram();

		tScreenBaseBlockIndex AllocBackgroundMem(u32 dataLengthAsU16, bool charBlockAligned);

	public:
		GBA::Gfx::tTileId AllocSpriteMem(const GBA::UPixelData* pixelMap, u32 pixelMapSize, u32 compressionFlags);
		void FreeSpriteMem(GBA::Gfx::tTileId index);

		/// <summary>
		/// Allocates a charblock to store tilesets in
		/// Better to call this before allocating screenblocks via AllocBackgroundTileMapMem
		/// This will let tilemap memory be allocated directly after the tileset
		/// If map memory is allocated at a CharBlock alignment then we lose a whole charblock to place tilesets at, which is bad
		/// </summary>
		/// <param name="tileSetLength">Number of tiles contained in the tileset to allocation</param>
		/// <returns>Returns a charblock index which points to an address in GBA background registers to store tilesets</returns>
		TileBlockGroups AllocBackgroundTileSetMem(u32 tileSetLength);

		void LoadBackgroundTileSetMem(
			const GBA::UPixelData* tileset,
			u32 tileSetLength,
			TileBlockGroups cbbIndex);

		void SetBackgroundTileSetMem(const GBA::UPixelData value, u32 tileSetLength, TileBlockGroups cbbIndex);

		/// <summary>
		/// Allocates screenblocks for tile map memory
		/// Better to call this after AllocBackgroundTileSetMem so as to not waste charblocks
		/// This will let tilemap memory be allocated directly after the tileset
		/// If map memory is allocated at a CharBlock alignment then we lose a whole charblock to place tilesets at, which is bad
		/// </summary>
		tScreenBaseBlockIndex AllocBackgroundTileMapMem(u32 tileCount);

		void LoadBackgroundTileMapMem(const BackgroundTilemapEntry* mapData,
			u32 mapDataLength,
			tScreenBaseBlockIndex sbbIndex);

		/// <summary>
		/// Vram memset a single tile
		/// </summary>
		void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, BackgroundTilemapEntry data);

		/// <summary>
		/// Vram memset the same data
		/// </summary>
		void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, BackgroundTilemapEntry data, int dataCount);

		/// <summary>
		/// Vram copy an array of data
		/// </summary>
		void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, const BackgroundTilemapEntry* data, int dataSize);

		void FreeBackgroundTileSetMem(TileBlockGroups cbbIndex);
		void FreeBackgroundTileMapMem(tScreenBaseBlockIndex sbbIndex);

		void Clear();

		static Vram& GetInstance()
		{
			static Vram instance; // Guaranteed to be destroyed.
								  // Instantiated on first use.
			return instance;
		}
	};
}
