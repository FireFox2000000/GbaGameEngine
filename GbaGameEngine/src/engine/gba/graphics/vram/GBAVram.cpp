#include "GBAVram.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/algorithm/Compression.h"
#include "engine/gba/registers/clock/GBATimer.h"

//#define DECOMPRESSION_PROFILE
//#define VRAM_TRANSFER_PROFILE
using namespace GBA::Gfx;

const int ScreenEntrySize = 2048;

using CharBlockPool = Array<GBA::Vram::CharBlock, GBA::BlockGroupCount>;
using CharBlockPool8 = Array<GBA::Vram::CharBlock8, GBA::BlockGroupCount>;

using ScreenBlock = Array<u16, ScreenEntrySize / sizeof(u16)>;
using ScreenBlockPool = Array<ScreenBlock, GBA::Vram::MaxScreenBlocks>;

const int ScreenBlocksPerCharBlock = 8;

namespace GBA
{
	volatile CharBlockPool& s_charBlockPool = *reinterpret_cast<volatile CharBlockPool*>(VRAM);
	volatile CharBlockPool8& s_charBlockPool8 = *reinterpret_cast<volatile CharBlockPool8*>(VRAM);
	volatile ScreenBlockPool& s_screenBlockPool = *reinterpret_cast<volatile ScreenBlockPool*>(VRAM);

	Vram::Vram()
		: m_spriteTileMemTracker(Free)
		, m_screenEntryTracker(Free)
	{
	}

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

		DEBUG_ERRORFORMAT("OUT OF VRAM! Failed to locate tile memory of size %d", tileCount);

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

	void Vram::LoadBackgroundMem(const u16* src, tScreenBaseBlockIndex dest, u32 dataLength)
	{
		u32 byteLength = dataLength * sizeof(u16);

#ifdef VRAM_TRANSFER_PROFILE
		auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
		profilerClock.SetFrequency(GBA::Timers::Cycle_64);

		profilerClock.SetActive(true);
#endif
		// Transfer memory
		VramSafeMemCopy((u16*)&s_screenBlockPool[dest][0], src, byteLength);
#ifdef VRAM_TRANSFER_PROFILE
		DEBUG_LOGFORMAT("[Profile VRAM transfer] = %d", profilerClock.GetCurrentTimerCount());
		profilerClock.SetActive(false);
#endif
	}

	void Vram::SetBackgroundMem(const u16 src, tScreenBaseBlockIndex dest, u32 dataLength)
	{
		u32 byteLength = dataLength * sizeof(u16);
		VramSafeMemSet((u16*)&s_screenBlockPool[dest][0], src, byteLength);
	}

	tScreenBaseBlockIndex Vram::AllocBackgroundMem(u32 dataLengthAsU16, bool charBlockAligned)
	{
		tScreenBaseBlockIndex allocatedIndex = INVALID_SBB_ID;
		u32 byteLength = dataLengthAsU16 * sizeof(u16);
		int totalScreenBlocksNeeded = ceil((float)byteLength / sizeof(ScreenBlock));
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

	TileBlockGroups Vram::AllocBackgroundTileSetMem(u32 tileSetLength)
	{
		u32 u16DataLength = tileSetLength * 2; // Tile set is stored as u32, but we process the data as u16
		tScreenBaseBlockIndex tileSbbIndex = AllocBackgroundMem(u16DataLength, true);
		if (tileSbbIndex == INVALID_SBB_ID)
		{
			DEBUG_ERROR("Unable to load background, out of memory for tileset");
			return TileBlockGroups::BlockGroupCount;
		}

		TileBlockGroups cbbIndex = static_cast<TileBlockGroups>(tileSbbIndex / ScreenBlocksPerCharBlock);
		DEBUG_LOGFORMAT("Loaded bg tileset into slot %d", (int)cbbIndex);

		return cbbIndex;
	}

	void Vram::LoadBackgroundTileSetMem(const u32 * tileset, u32 tileSetLength, TileBlockGroups cbbIndex)
	{
		u32 dataLength = tileSetLength * 2;	// We cast the data from a u32[] to a u16[]. Double the length to account for this.
		LoadBackgroundMem((u16*)tileset, cbbIndex * ScreenBlocksPerCharBlock, dataLength);
	}

	void Vram::SetBackgroundTileSetMem(const u16 value, u32 tileSetLength, TileBlockGroups cbbIndex)
	{
		u32 dataLength = tileSetLength * 2;	// We cast the data from a u32[] to a u16[]. Double the length to account for this.
		SetBackgroundMem(value, cbbIndex * ScreenBlocksPerCharBlock, dataLength);
	}

	tScreenBaseBlockIndex Vram::AllocBackgroundTileMapMem(u32 tileCount)
	{
		tScreenBaseBlockIndex sbbIndex = AllocBackgroundMem(tileCount, false);

		DEBUG_ASSERTMSG(sbbIndex != INVALID_SBB_ID, "Unable to load background, out of memory for map");
		DEBUG_LOGFORMAT("Loaded map into slot %d", (int)sbbIndex);

		return sbbIndex;
	}

	void Vram::LoadBackgroundTileMapMem(const u16 * mapData, u32 mapDataLength, tScreenBaseBlockIndex sbbIndex)
	{
		LoadBackgroundMem(mapData, sbbIndex, mapDataLength);
	}

	void Vram::SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, u16 data)
	{
		s_screenBlockPool[sbbIndex][offset] = data;
	}

	void Vram::SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, u16 data, int dataCount)
	{
		VramSafeMemSet((u16*)&s_screenBlockPool[sbbIndex][offset], data, dataCount);
	}

	void Vram::SetBackgroundTileData(tScreenBaseBlockIndex sbbIndex, u32 offset, const u16* data, int dataSize)
	{
		VramSafeMemCopy((u16*)&s_screenBlockPool[sbbIndex][offset], data, sizeof(u16) * dataSize);
	}

	void Vram::FreeBackgroundTileSetMem(TileBlockGroups cbbIndex)
	{
		if (cbbIndex == TileBlockGroups::BlockGroupCount) return;

		u32 cbbSeIndex = cbbIndex * ScreenBlocksPerCharBlock;
		FreeBackgroundTileMapMem(cbbSeIndex);
	}

	void Vram::FreeBackgroundTileMapMem(tScreenBaseBlockIndex sbbIndex)
	{
		if (sbbIndex == INVALID_SBB_ID) return;

		m_screenEntryTracker[sbbIndex++] = Free;

		while (sbbIndex < m_screenEntryTracker.Count() && m_screenEntryTracker[sbbIndex] == Continue)
		{
			m_screenEntryTracker[sbbIndex++] = Free;
		}
	}

	void Vram::Clear()
	{
		m_spriteTileMemTracker.SetAllTo(AllocState::Free);
		m_screenEntryTracker.SetAllTo(AllocState::Free);
	}

	volatile GBA::Vram::CharBlock* Vram::EditTileBlock(TileBlockGroups group)
	{
		return &(s_charBlockPool[int(group)]);
	}

	volatile GBA::Vram::CharBlock8* Vram::EditTileBlock8(TileBlockGroups group)
	{
		return &(s_charBlockPool8[int(group)]);
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
