#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/tiles/GBATile.h"

namespace GBA
{
	// Charblocks/Character base block: region of memory for tilesets to be placed. 
	// ScreenBlocks: region of memory for screen entires to be placed.

	enum TileBlockGroups
	{
		Bg0,
		Bg1,
		Bg2,
		Bg3,
		SpriteLower,		// May be used by backgrounds in bitmap modes
		SpriteHigher,

		BlockGroupCount
	};

	using tScreenBaseBlockIndex = u16;
	const tScreenBaseBlockIndex INVALID_SBB_ID = -1;

	typedef List<u32> tSpriteData;

	class Vram
	{
		static constexpr int CharBlockSize = 512;
		static const int CharBlock8Size = 256;

	public:
		using CharBlock = Array<GBA::Gfx::Tile::Tile, CharBlockSize>;
		using CharBlock8 = Array<GBA::Gfx::Tile::Tile8, CharBlock8Size>;
		static const u32 MaxScreenBlocks = 32;

	private:
		static volatile CharBlock* EditTileBlock(TileBlockGroups group);
		static volatile CharBlock8* EditTileBlock8(TileBlockGroups group);

		bool LoadTiles(const u32* pixelMap, u32 pixelMapSize, u32 compressionFlags, TileBlockGroups tileBlockGroup, u16 startTileIndex);

		enum AllocState
		{
			Free,
			Used,
			Continue
		};

		// Sprite tile mem allocator
		// See "OBJ Sprite Memory Management" - https://www.gamasutra.com/view/feature/131491/gameboy_advance_resource_management.php?print=1
		static const u32 MAX_SPRITE_TILES = 1024;
		Array<AllocState, MAX_SPRITE_TILES> m_spriteTileMemTracker;
		GBA::Gfx::tTileId FindNextFreeSpriteTileSpace(u8 tileCount) const;

		// Background tile mem allocator
		Array<AllocState, MaxScreenBlocks> m_screenEntryTracker;

		Vram();

		void LoadBackgroundMem(const u16* src, tScreenBaseBlockIndex dest, u32 dataLength);
		void SetBackgroundMem(const u16 src, tScreenBaseBlockIndex dest, u32 dataLength);
		tScreenBaseBlockIndex AllocBackgroundMem(u32 dataLengthAsU16, bool charBlockAligned);

	public:
		GBA::Gfx::tTileId AllocSpriteMem(const u32* pixelMap, u32 pixelMapSize, u32 compressionFlags);
		void FreeSpriteMem(GBA::Gfx::tTileId index);

		/// <summary>
		/// Allocates a charblock to store tilesets in
		/// Better to call this before allocating screenblocks via AllocBackgroundTileMapMem
		/// This will let tilemap memory be allocated directly after the tileset
		/// If map memory is allocated at a CharBlock alignment then we lose a whole charblock to place tilesets at, which is bad
		/// </summary>
		/// <param name="tileSetLength">Number of tiles contained in the tileset to allocation</param>
		/// <returns>Returns a charblock index which points to an address in GBA background registers to store tilesets</returns>
		TileBlockGroups AllocBackgroundTileSetMem(
			u32 tileSetLength);

		void LoadBackgroundTileSetMem(
			const u32* tileset,
			u32 tileSetLength,
			TileBlockGroups cbbIndex);

		void SetBackgroundTileSetMem(const u16 value, u32 tileSetLength, TileBlockGroups cbbIndex);

		/// <summary>
		/// Allocates screenblocks for tile map memory
		/// Better to call this after AllocBackgroundTileSetMem so as to not waste charblocks
		/// This will let tilemap memory be allocated directly after the tileset
		/// If map memory is allocated at a CharBlock alignment then we lose a whole charblock to place tilesets at, which is bad
		/// </summary>
		tScreenBaseBlockIndex AllocBackgroundTileMapMem(u32 tileCount);

		void LoadBackgroundTileMapMem(const u16* mapData,
			u32 mapDataLength,
			tScreenBaseBlockIndex sbbIndex);

		void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, u16 data);
		void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, const u16* data, int dataSize);

		/*void AllocBackgroundMem(
			const u32* tileset,
			u32 tileSetLength, 
			const u16* mapData,
			u32 mapDataLength, 
			TileBlockGroups& out_cbbIndex, 
			tScreenBaseBlockIndex& out_sbbIndex);*/

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
