#include "GBAVram.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/algorithm/Compression.h"
#include "engine/gba/registers/clock/GBATimer.h"

//#define DECOMPRESSION_PROFILE

namespace GBA
{
	volatile Vram::CharBlockPool& Vram::s_charBlockPool = *reinterpret_cast<volatile CharBlockPool*>(VRAM);
	volatile Vram::CharBlockPool8& Vram::s_charBlockPool8 = *reinterpret_cast<volatile CharBlockPool8*>(VRAM);

	tTileId Vram::FindNextFreeSpriteTileSpace(u8 tileCount) const
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

		return INVALID_TILE_ID;
	}

	tTileId Vram::AllocSpriteMem(const u32 * pixelMap, u32 pixelMapSize, u32 compressionFlags)
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

	void Vram::FreeSpriteMem(tTileId index)
	{
		m_spriteTileMemTracker[index++] = Free;

		while (index < m_spriteTileMemTracker.Count() && m_spriteTileMemTracker[index] == Continue)
		{
			m_spriteTileMemTracker[index++] = Free;
		}
	}

	bool Vram::LoadTiles(const u32 * pixelMap, u32 pixelMapSize, u32 compressionFlags, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	{
		volatile Tile::Tile* tileBlock = EditTileBlock(tileBlockGroup)->At(0);
		vu32 *tileMem = reinterpret_cast<vu32*>(tileBlock + startTileIndex);			// Allow tiles to bleed over into further char blocks

		Compression::Type compressionType = Compression::GetCompressionType(compressionFlags);
		u8 bitPackedSrcBpp = Compression::GetBitPackedSrcBpp(compressionFlags);

		if (compressionType == Compression::Type::BitPacked)
		{
#ifdef DECOMPRESSION_PROFILE
			auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
			profilerClock.SetFrequency(GBA::Timers::Cycle_64);
			profilerClock.SetActive(true);
#endif
			Compression::BitUnpack((void*)tileMem, pixelMap, pixelMapSize * sizeof(pixelMapSize), bitPackedSrcBpp, 4);
#ifdef DECOMPRESSION_PROFILE
			DEBUG_LOGFORMAT("[Profile Compression::BitUnpack] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);
#endif
			return true;
		}
		// Compression::Type::None
		else if ((void*)(tileMem + pixelMapSize) <= (void*)s_charBlockPool.end())
		{
#ifdef DECOMPRESSION_PROFILE
			auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
			profilerClock.SetFrequency(GBA::Timers::Cycle_64);
			profilerClock.SetActive(true);
#endif
			// Just throw it directly in
			for (u32 i = 0; i < pixelMapSize; ++i)
			{
				tileMem[i] = *(pixelMap + i);
			}
#ifdef DECOMPRESSION_PROFILE
			DEBUG_LOGFORMAT("[Profile direct data copy] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);
#endif
			return true;
		}

		return false; // NOT ENOUGH SPACE, OUT OF VRAM!
	}

	//bool TileBank::LoadTiles(const List<u32>& pixelMap, TileBlockGroups tileBlockGroup, u16 startTileIndex)
	//{
	//	return LoadTiles(pixelMap.begin(), pixelMap.Count(), tileBlockGroup, startTileIndex);
	//}
	//
	//bool TileBank::LoadSpriteTiles(const tSpriteData& pixelMap, tTileId tileId)
	//{
	//	if (tileId < CharBlockSize)
	//		return LoadTiles(pixelMap, SpriteLower, tileId);
	//	else
	//		return LoadTiles(pixelMap, SpriteHigher, tileId - CharBlockSize);
	//}
}
