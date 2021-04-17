#pragma once

#include "engine/base/Typedefs.h"
#include "engine/base/core/stl/Array.h"
#include "engine/base/core/stl/List.h"
#include "engine/gba/graphics/tiles/GBATile.h"

namespace GBA
{
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
		static const int CharBlockSize = 512;
		static const int CharBlock8Size = 256;
		static const int ScreenEntrySize = 2048;
		static const u32 MaxScreenBlocks = 32;

		typedef Array<GBA::Gfx::Tile::Tile, CharBlockSize> CharBlock;
		typedef Array<GBA::Gfx::Tile::Tile8, CharBlock8Size> CharBlock8;
		typedef Array<CharBlock, BlockGroupCount> CharBlockPool;
		typedef Array<CharBlock8, BlockGroupCount> CharBlockPool8;

		typedef Array<u16, ScreenEntrySize / sizeof(u16)> ScreenBlock;
		typedef Array<ScreenBlock, MaxScreenBlocks> ScreenBlockPool;

		static volatile CharBlockPool & s_charBlockPool;
		static volatile CharBlockPool8 & s_charBlockPool8;

		static volatile ScreenBlockPool & s_screenBlockPool;

		static volatile CharBlock* EditTileBlock(TileBlockGroups group) { return &(s_charBlockPool[int(group)]); }
		static volatile CharBlock8* EditTileBlock8(TileBlockGroups group) { return &(s_charBlockPool8[int(group)]); }

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
		tScreenBaseBlockIndex AllocBackgroundMem(u32 dataLengthAsU16, bool charBlockAligned);

	public:
		GBA::Gfx::tTileId AllocSpriteMem(const u32* pixelMap, u32 pixelMapSize, u32 compressionFlags);
		void FreeSpriteMem(GBA::Gfx::tTileId index);

		TileBlockGroups AllocBackgroundTileSetMem(
			u32 tileSetLength);
		void LoadBackgroundTileSetMem(
			const u32* tileset,
			u32 tileSetLength,
			TileBlockGroups cbbIndex);

		tScreenBaseBlockIndex AllocBackgroundTileMapMem(u32 tileCount);
		void LoadBackgroundTileMapMem(const u16* mapData,
			u32 mapDataLength,
			tScreenBaseBlockIndex sbbIndex);

		inline void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, u16 data) { s_screenBlockPool[sbbIndex][offset] = data; }
		inline void SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, const u16* data, int dataSize) { VramSafeMemCopy(data, (u16*)&s_screenBlockPool[sbbIndex][offset], sizeof(u16) *dataSize); }

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
