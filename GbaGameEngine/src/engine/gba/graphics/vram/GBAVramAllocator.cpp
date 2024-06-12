#include "GBAVramAllocator.h"
#include "engine/algorithm/Compression.h"

//#define DECOMPRESSION_PROFILE
//#define VRAM_TRANSFER_PROFILE

#if defined(DECOMPRESSION_PROFILE) || defined(VRAM_TRANSFER_PROFILE)
#include "engine/gba/config/GBATimerId.h"
#include "gbatek/Timers.h"
#endif

using namespace GBA::Gfx;

constexpr int ScreenBlocksPerCharBlock = 
	ARRAY_SIZE(GBA::VramTileMode::UBackgroundMapsAndTiles::screenBaseBlocks) / 
	ARRAY_SIZE(GBA::VramTileMode::UBackgroundMapsAndTiles::characterBaseBlocks);

namespace GBA
{
	VramAllocator::VramAllocator()
		: m_spriteTileMemTracker(Free)
		, m_screenEntryTracker(Free)
	{
	}

	tTileId VramAllocator::FindNextFreeSpriteTileSpace(u8 tileCount) const
	{
		tTileId tileIndex = tileCount;		// We start from offset of a whole tile because tile 0 is reserved for disabled tiles

		while (tTileId(tileIndex + tileCount) < m_spriteTileMemTracker.Count())
		{
			bool tileSpaceValid = true;

			for (u16 i = tileIndex; i < (tileIndex + tileCount); ++i)		// Check that the space for the tile actually exists
			{
				if (m_spriteTileMemTracker[i] != Free)
				{
					tileSpaceValid = false;
					break;
				}
			}

			if (tileSpaceValid)
				return tileIndex;
			else
				tileIndex += tileCount;	// Align to the size to prevent fragmentation
		}

		DEBUG_ERRORFORMAT("OUT OF VRAM! Failed to locate tile memory of size %d", tileCount);

		return INVALID_TILE_ID;
	}

	tTileId VramAllocator::AllocSpriteMem(const GBA::UPixelData* pixelMap, u32 pixelMapSize, u32 compressionFlags)
	{
		Compression::Type compressionType = Compression::GetCompressionType(compressionFlags);

		// Set tiles
		const u8 bitsPerByte = 8;
		u8 bitsPerPixel = 4;

		if (compressionType == Compression::Type::BitPacked)
		{
			bitsPerPixel = Compression::GetBitPackedSrcBpp(compressionFlags);
		}

		u8 pixelsPerByte = bitsPerByte / bitsPerPixel;
		u16 totalBytes = pixelMapSize * sizeof(pixelMapSize);
		u16 totalPixels = totalBytes * pixelsPerByte;
		u8 tileCount = totalPixels / Tile::PIXELS_PER_TILE;

		tTileId tileIndex = FindNextFreeSpriteTileSpace(tileCount);
		if (tileIndex != INVALID_TILE_ID)
		{
			// This should be set to sprite higher in bitmap modes
			TileBlockGroups tileBlockGroup = SpriteLower;

			LoadTiles(pixelMap, pixelMapSize, compressionFlags, tileBlockGroup, tileIndex);

			m_spriteTileMemTracker[tileIndex] = Used;
			for (int i = tileIndex + 1; i < tileIndex + tileCount; ++i)
			{
				m_spriteTileMemTracker[i] = Continue;
			}
		}

		return tileIndex;
	}

	void VramAllocator::FreeSpriteMem(tTileId index)
	{
		m_spriteTileMemTracker[index++] = Free;

		while (index < m_spriteTileMemTracker.Count() && m_spriteTileMemTracker[index] == Continue)
		{
			m_spriteTileMemTracker[index++] = Free;
		}
	}

	VramAllocator::tScreenBaseBlockIndex VramAllocator::AllocBackgroundMem(u32 dataLengthAsU16, bool charBlockAligned)
	{
		tScreenBaseBlockIndex allocatedIndex = INVALID_SBB_ID;
		u32 byteLength = dataLengthAsU16 * sizeof(u16);
		constexpr int screenBlockSize = sizeof(GBA::vram->videoMode2.backgroundMapsAndTiles.screenBaseBlocks[0]);
		int totalScreenBlocksNeeded = ceil((float)byteLength / screenBlockSize);
		int sbbOffset = charBlockAligned ? ScreenBlocksPerCharBlock : 1;

		// Find the next free slots
		for (u32 sbbIndex = 0; sbbIndex < m_screenEntryTracker.Count(); sbbIndex += sbbOffset)
		{
			bool sbbSpaceValid = true;

			for (u16 i = sbbIndex; i < (sbbIndex + totalScreenBlocksNeeded); ++i)		// Check that the space for the tile actually exists
			{
				if (m_screenEntryTracker[i] != Free)
				{
					sbbSpaceValid = false;
					if (!charBlockAligned)
						sbbIndex = i;

					break;
				}
			}

			if (sbbSpaceValid)
			{
				allocatedIndex = sbbIndex;
				break;
			}
		}

		if (allocatedIndex == INVALID_SBB_ID)
			return INVALID_SBB_ID;

		// Mark the memory slots as used
		m_screenEntryTracker[allocatedIndex] = Used;
		for (u16 i = allocatedIndex + 1; i < (allocatedIndex + totalScreenBlocksNeeded); ++i)		// Check that the space for the tile actually exists
		{
			m_screenEntryTracker[i] = Continue;
		}

		return allocatedIndex;
	}

	TileBlockGroups VramAllocator::AllocBackgroundTileSetMem(u32 tileSetLength)
	{
		u32 u16DataLength = tileSetLength * 2; // Tile set is stored as u32, but we process the data as u16
		tScreenBaseBlockIndex tileSbbIndex = AllocBackgroundMem(u16DataLength, true);
		if (tileSbbIndex == INVALID_SBB_ID)
		{
			DEBUG_ERROR("Unable to load background, out of memory for tileset");
			return TileBlockGroups::BlockGroupCount;
		}

		TileBlockGroups cbbIndex = static_cast<TileBlockGroups>(GBA::VramTileMode::ScreenBaseBlockToCharacterBaseBlock(tileSbbIndex));
		DEBUG_LOGFORMAT("Loaded bg tileset into slot %d", (int)cbbIndex);

		return cbbIndex;
	}

	void VramAllocator::LoadBackgroundTileSetMem(const GBA::UPixelData* tileset, u32 tileSetLength, TileBlockGroups cbbIndex)
	{
		auto* dest = GBA::vram->videoMode2.backgroundMapsAndTiles.characterBaseBlocks[cbbIndex];
		VramSafeMemCopy(dest->pixelData, tileset, tileSetLength);
	}

	void VramAllocator::SetBackgroundTileSetMem(const GBA::UPixelData value, u32 tileSetLength, TileBlockGroups cbbIndex)
	{
		auto* dest = GBA::vram->videoMode2.backgroundMapsAndTiles.characterBaseBlocks[cbbIndex];
		VramSafeMemSet(dest->pixelData, value, tileSetLength);
	}

	VramAllocator::tScreenBaseBlockIndex VramAllocator::AllocBackgroundTileMapMem(u32 tileCount)
	{
		tScreenBaseBlockIndex sbbIndex = AllocBackgroundMem(tileCount, false);

		DEBUG_ASSERTMSG(sbbIndex != INVALID_SBB_ID, "Unable to load background, out of memory for map");
		DEBUG_LOGFORMAT("Loaded map into slot %d", (int)sbbIndex);

		return sbbIndex;
	}

	void VramAllocator::LoadBackgroundTileMapMem(const BackgroundTilemapEntry* mapData, u32 mapDataLength, tScreenBaseBlockIndex sbbIndex)
	{
		auto* dest = GBA::vram->videoMode2.backgroundMapsAndTiles.screenBaseBlocks[sbbIndex];
		u32 byteLength = mapDataLength * sizeof(*mapData);
		VramSafeMemCopy(dest, mapData, byteLength);
	}

	void VramAllocator::SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, BackgroundTilemapEntry data)
	{
		GBA::vram->videoMode2.backgroundMapsAndTiles.screenBaseBlocks[sbbIndex][offset] = data;
	}

	void VramAllocator::SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, BackgroundTilemapEntry data, int dataCount)
	{
		auto* dest = &(GBA::vram->videoMode2.backgroundMapsAndTiles.screenBaseBlocks[sbbIndex][offset]);
		VramSafeMemSet(dest, data, dataCount);
	}

	void VramAllocator::SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, const BackgroundTilemapEntry* data, int dataSize)
	{
		auto* dest = &(GBA::vram->videoMode2.backgroundMapsAndTiles.screenBaseBlocks[sbbIndex][offset]);
		VramSafeMemCopy(dest, data, dataSize);
	}

	void VramAllocator::FreeBackgroundTileSetMem(TileBlockGroups cbbIndex)
	{
		if (cbbIndex == TileBlockGroups::BlockGroupCount) return;

		u32 cbbSeIndex = GBA::VramTileMode::CharacterBaseBlockToScreenBaseBlock(cbbIndex);
		FreeBackgroundTileMapMem(cbbSeIndex);
	}

	void VramAllocator::FreeBackgroundTileMapMem(tScreenBaseBlockIndex sbbIndex)
	{
		if (sbbIndex == INVALID_SBB_ID) return;

		m_screenEntryTracker[sbbIndex++] = Free;

		while (sbbIndex < m_screenEntryTracker.Count() && m_screenEntryTracker[sbbIndex] == Continue)
		{
			m_screenEntryTracker[sbbIndex++] = Free;
		}
	}

	void VramAllocator::Clear()
	{
		m_spriteTileMemTracker.SetAllTo(AllocState::Free);
		m_screenEntryTracker.SetAllTo(AllocState::Free);
	}

	bool VramAllocator::LoadTiles(const GBA::UPixelData* pixelMap, u32 pixelMapSize, u32 compressionFlags, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		GBA::BaseTile* tileBlock = nullptr;

		if (tileBlockGroup >= TileBlockGroups::SpriteLower)
		{
			tileBlock = GBA::vram->videoMode2.objectTiles;
		}
		else
		{
			tileBlock = GBA::vram->videoMode2.backgroundMapsAndTiles.characterBaseBlocks[tileBlockGroup];
		}

		// Allow tiles to bleed over into further char blocks
		GBA::UPixelData* tileMem = (tileBlock + startTileIndex)->pixelData;

		Compression::Type compressionType = Compression::GetCompressionType(compressionFlags);
		u8 bitPackedSrcBpp = Compression::GetBitPackedSrcBpp(compressionFlags);

		if (compressionType == Compression::Type::BitPacked)
		{
#ifdef DECOMPRESSION_PROFILE
			auto& profilerClock = GBA::ioRegisterTimers->at(GBATimerId::Profile);
			profilerClock.frequency = GBA::ClockFrequency::Cycle_64;
			profilerClock.isEnabled = true;
#endif
			Compression::BitUnpack(tileMem, pixelMap, pixelMapSize * sizeof(pixelMapSize), bitPackedSrcBpp, 4);
#ifdef DECOMPRESSION_PROFILE
			DEBUG_LOGFORMAT("[Profile Compression::BitUnpack] = %d", profilerClock.GetCurrentCount());
			profilerClock.isEnabled = false;
#endif
			return true;
		}
		// Compression::Type::None
		else if ((u8*)(tileMem + pixelMapSize) <= ((u8*)(GBA::vram) + sizeof(GBA::vram->videoMode2)))
		{
#ifdef DECOMPRESSION_PROFILE
			auto& profilerClock = GBA::ioRegisterTimers->at(GBATimerId::Profile);
			profilerClock.frequency = GBA::ClockFrequency::Cycle_64;
			profilerClock.isEnabled = true;
#endif
			// Just throw it directly in
			for (u32 i = 0; i < pixelMapSize; ++i)
			{
				tileMem[i] = *(pixelMap + i);
			}
#ifdef DECOMPRESSION_PROFILE
			DEBUG_LOGFORMAT("[Profile direct data copy] = %d", profilerClock.GetCurrentCount());
			profilerClock.isEnabled = false;
#endif
			return true;
		}

		DEBUG_ASSERTMSG(false, "OUT OF VRAM");
		return false; // NOT ENOUGH SPACE, OUT OF VRAM!
	}
}
