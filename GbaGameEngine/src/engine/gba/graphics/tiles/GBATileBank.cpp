#include "GBATileBank.h"
#include "engine/gba/memory/GBAMemoryLocations.h"
#include "engine/algorithm/Compression.h"
#include "engine/gba/registers/clock/GBATimer.h"

//#define DECOMPRESSION_PROFILE

namespace GBA
{
	volatile TileBank::CharBlockPool& TileBank::s_charBlockPool = *reinterpret_cast<volatile CharBlockPool*>(VRAM);
	volatile TileBank::CharBlockPool8& TileBank::s_charBlockPool8 = *reinterpret_cast<volatile CharBlockPool8*>(VRAM);

	bool TileBank::LoadTiles(const u32 * pixelMap, u32 pixelMapSize, u32 compressionFlags, TileBlockGroups tileBlockGroup, u16 startTileIndex)
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
