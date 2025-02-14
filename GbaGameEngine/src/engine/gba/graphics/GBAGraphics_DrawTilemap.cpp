#include "GBAGraphics.h"
#include "engine/gba/graphics/tilemap/GBATilemap.h"
#include "engine/math/Vector2.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/screen/Screen.h"
#include "engine/time/Time.h"
#include "engine/debug/Profiler.h"
#include "gbatek/Vram.h"
#include "gbatek/Backgrounds.h"

//#define LOG_RENDER_ROWCOLS
//#define PROFILE_RENDER

struct MapWrappingPoints
{
	struct WrappingPointsX
	{
		int bgTileXStart;
		int bgTileXEnd;
		int xWrappingOffsetPoint;
		int tilemapXStart;
	};

	struct WrappingPointsY
	{
		int bgTileYStart;
		int bgTileYEnd;
		int yWrappingOffsetPoint;

		int tilemapYStart;
		int tilemapYEnd;
		int tilemapYWrappingOffsetPoint;
	};

	WrappingPointsX allColumn;
	WrappingPointsY allRow;

	WrappingPointsX newColumn;
	WrappingPointsY newRow;
};

// Faster than Math::Mod when you know for sure that x isn't too far from 0 or N.
int LoopMod(int x, unsigned int N)
{
	if (x < 0)
	{
		while (x < 0)
		{
			x += N;
		}

		return x;
	}
	else
	{
		while (x >= (int)N)
		{
			x -= N;
		}

		return x;
	}
}

MapWrappingPoints CalculateMapWrappingPoints(
	const Vector2i& tilemapRenderStartPos
	, const Vector2i& renderSize
	, const Vector2<u8>& tileMapSizeInTiles
	, const Vector2i& lastRenderPos
	, bool lastRenderPosValid
)
{
	using namespace GBA;
	using namespace GBA::Gfx;

	Vector2i deltaPos = tilemapRenderStartPos - lastRenderPos;

	Vector2i tilemapRenderStartPosMod;
	tilemapRenderStartPosMod.x = Math::Mod(tilemapRenderStartPos.x, tileMapSizeInTiles.x);
	tilemapRenderStartPosMod.y = Math::Mod(tilemapRenderStartPos.y, tileMapSizeInTiles.y);

	// Cache where we need to wrap indexes. Can't transfer outside tilemap or 32x32 BG range etc, and modulus is expensive in a loop.
	int bgTileXStartAll = 0, bgTileXEndAll = 0;
	int bgTileXStartNew = 0, bgTileXEndNew = 0;

	bgTileXStartAll = Math::Power2Mod(tilemapRenderStartPos.x, TilemapManager::VARIABLE_TILEMAP_SIZE.x);
	bgTileXEndAll = Math::Power2Mod((tilemapRenderStartPos.x + renderSize.x), TilemapManager::VARIABLE_TILEMAP_SIZE.x);

	if (lastRenderPosValid && deltaPos.x != 0)
	{
		// Only render the new tiles on the screen
		if (deltaPos.x > 0)
		{
			bgTileXStartNew = Math::Power2Mod(tilemapRenderStartPos.x + renderSize.x - deltaPos.x, TilemapManager::VARIABLE_TILEMAP_SIZE.x);
			bgTileXEndNew = bgTileXEndAll;
		}
		else
		{
			bgTileXStartNew = bgTileXStartAll;
			bgTileXEndNew = Math::Power2Mod((tilemapRenderStartPos.x - deltaPos.x), TilemapManager::VARIABLE_TILEMAP_SIZE.x);
		}
	}


	int xWrappingOffsetPointAll = TilemapManager::VARIABLE_TILEMAP_SIZE.x;
	if (bgTileXEndAll > bgTileXStartAll)	// No wrapping occoured
	{
		xWrappingOffsetPointAll = bgTileXEndAll;
		bgTileXEndAll = 0;
	}

	int xWrappingOffsetPointNew = TilemapManager::VARIABLE_TILEMAP_SIZE.x;
	if (bgTileXEndNew > bgTileXStartNew)	// No wrapping occoured
	{
		xWrappingOffsetPointNew = bgTileXEndNew;
		bgTileXEndNew = 0;
	}

	int tilemapXStartAll = 0, tilemapXStartNew = 0;
	{
		tilemapXStartAll = LoopMod(tilemapRenderStartPosMod.x, tileMapSizeInTiles.x);
	}
	if (lastRenderPosValid && deltaPos.x > 0)
	{
		// Only render the new tiles on the screen
		tilemapXStartNew = LoopMod(tilemapRenderStartPosMod.x + renderSize.x - deltaPos.x, tileMapSizeInTiles.x);
	}
	else
	{
		tilemapXStartNew = tilemapXStartAll;
	}

	int bgTileYStartAll = 0, bgTileYEndAll = 0;
	int bgTileYStartNew = 0, bgTileYEndNew = 0;

	{
		bgTileYStartAll = Math::Power2Mod(tilemapRenderStartPos.y, TilemapManager::VARIABLE_TILEMAP_SIZE.y);
		bgTileYEndAll = Math::Power2Mod((tilemapRenderStartPos.y + renderSize.y), TilemapManager::VARIABLE_TILEMAP_SIZE.y);
	}

	if (lastRenderPosValid && deltaPos.y != 0)
	{
		// Only render the new tiles on the screen
		if (deltaPos.y > 0)
		{
			bgTileYStartNew = Math::Power2Mod(tilemapRenderStartPos.y + renderSize.y - deltaPos.y, TilemapManager::VARIABLE_TILEMAP_SIZE.y);
			bgTileYEndNew = bgTileYEndAll;
		}
		else
		{
			bgTileYStartNew = bgTileYStartAll;
			bgTileYEndNew = Math::Power2Mod((tilemapRenderStartPos.y - deltaPos.y), TilemapManager::VARIABLE_TILEMAP_SIZE.y);
		}
	}

	int yWrappingOffsetPointAll = TilemapManager::VARIABLE_TILEMAP_SIZE.y;
	if (bgTileYEndAll > bgTileYStartAll)	// No wrapping occoured
	{
		yWrappingOffsetPointAll = bgTileYEndAll;
		bgTileYEndAll = 0;
	}

	if (lastRenderPosValid && deltaPos.x == 0)
	{
		bgTileYStartAll = 0;
		bgTileYEndAll = 0;
		yWrappingOffsetPointAll = 0;
	}

	int yWrappingOffsetPointNew = TilemapManager::VARIABLE_TILEMAP_SIZE.y;
	if (bgTileYEndNew > bgTileYStartNew)	// No wrapping occoured
	{
		yWrappingOffsetPointNew = bgTileYEndNew;
		bgTileYEndNew = 0;
	}

	int tilemapYStartAll = 0, tilemapYStartNew = 0;
	{
		tilemapYStartAll = LoopMod(tilemapRenderStartPosMod.y, tileMapSizeInTiles.y);
	}
	if (lastRenderPosValid && deltaPos.y > 0)
	{
		// Only render the new tiles on the screen
		tilemapYStartNew = LoopMod(tilemapRenderStartPosMod.y + renderSize.y - deltaPos.y, tileMapSizeInTiles.y);
	}
	else
	{
		tilemapYStartNew = tilemapYStartAll;
	}

	int tilemapYEndAll = LoopMod(tilemapRenderStartPosMod.y + renderSize.y, tileMapSizeInTiles.y);
	int tilemapYEndNew = tilemapYEndAll;

	int tilemapYWrappingOffsetPointAll = tileMapSizeInTiles.y;
	int tilemapYWrappingOffsetPointNew = tilemapYWrappingOffsetPointAll;

	if (tilemapYEndAll > tilemapYStartAll)	// No wrapping occoured
	{
		tilemapYWrappingOffsetPointAll = tilemapYEndAll;
		tilemapYEndAll = 0;
	}

	if (tilemapYEndNew > tilemapYStartNew)	// No wrapping occoured
	{
		tilemapYWrappingOffsetPointNew = tilemapYEndNew;
		tilemapYEndNew = 0;
	}

	if (lastRenderPosValid && deltaPos.y == 0)
	{
		bgTileYStartNew = 0;
		bgTileYEndNew = 0;
		yWrappingOffsetPointNew = 0;
	}

	if (lastRenderPosValid && deltaPos.x == 0)
	{
		bgTileXStartNew = 0;
		bgTileXEndNew = 0;
		xWrappingOffsetPointNew = 0;
	}

	MapWrappingPoints wrappingPoints;

	wrappingPoints.allColumn.bgTileXStart = bgTileXStartAll;
	wrappingPoints.allColumn.bgTileXEnd = bgTileXEndAll;
	wrappingPoints.allColumn.xWrappingOffsetPoint = xWrappingOffsetPointAll;
	wrappingPoints.allColumn.tilemapXStart = tilemapXStartAll;

	wrappingPoints.newColumn.bgTileXStart = bgTileXStartNew;
	wrappingPoints.newColumn.bgTileXEnd = bgTileXEndNew;
	wrappingPoints.newColumn.xWrappingOffsetPoint = xWrappingOffsetPointNew;
	wrappingPoints.newColumn.tilemapXStart = tilemapXStartNew;

	wrappingPoints.allRow.bgTileYStart = bgTileYStartAll;
	wrappingPoints.allRow.bgTileYEnd = bgTileYEndAll;
	wrappingPoints.allRow.yWrappingOffsetPoint = yWrappingOffsetPointAll;
	wrappingPoints.allRow.tilemapYStart = tilemapYStartAll;
	wrappingPoints.allRow.tilemapYEnd = tilemapYEndAll;
	wrappingPoints.allRow.tilemapYWrappingOffsetPoint = tilemapYWrappingOffsetPointAll;

	wrappingPoints.newRow.bgTileYStart = bgTileYStartNew;
	wrappingPoints.newRow.bgTileYEnd = bgTileYEndNew;
	wrappingPoints.newRow.yWrappingOffsetPoint = yWrappingOffsetPointNew;
	wrappingPoints.newRow.tilemapYStart = tilemapYStartNew;
	wrappingPoints.newRow.tilemapYEnd = tilemapYEndNew;
	wrappingPoints.newRow.tilemapYWrappingOffsetPoint = tilemapYWrappingOffsetPointNew;

	return wrappingPoints;
}

static inline void CopyFromMapToVramSingle(
	GBA::VramAllocator& vram
	, GBA::VramAllocator::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgCol
	, const GBATEK::BackgroundTilemapEntry* srcMapData
	, int srcMapIndex
	, int size		// Unused, just for compatibility with the other functions. Should be equal to exactly 1. 
)
{
	u32 offset = destBgRow * GBA::Gfx::TilemapManager::VARIABLE_TILEMAP_SIZE.x + destBgCol;
	vram.SetBackgroundTileData(sbbIndex, offset, srcMapData[srcMapIndex]);
}

static inline void CopyFromMapToVramLoop(
	GBA::VramAllocator& vram
	, GBA::VramAllocator::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgCol
	, const GBATEK::BackgroundTilemapEntry* srcMapData
	, int srcMapIndex
	, int size
)
{
	u32 offset = destBgRow * GBA::Gfx::TilemapManager::VARIABLE_TILEMAP_SIZE.x + destBgCol;
	for (int i = 0; i < size; ++i)
	{
		vram.SetBackgroundTileData(sbbIndex, offset + i, srcMapData[srcMapIndex + i]);
	}
}

static inline void CopyFromMapToVramMemCpy(
	GBA::VramAllocator& vram
	, GBA::VramAllocator::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgCol
	, const GBATEK::BackgroundTilemapEntry* srcMapData
	, int srcMapIndex
	, int size
)
{
	u32 offset = destBgRow * GBA::Gfx::TilemapManager::VARIABLE_TILEMAP_SIZE.x + destBgCol;
	vram.SetBackgroundTileData(sbbIndex, offset, &srcMapData[srcMapIndex], size);
}

// Pass function via template rather than function pointer so it can be inlined for SPEEEEEED
typedef void(*Fn)(GBA::VramAllocator& vram
	, GBA::VramAllocator::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgCol
	, const GBATEK::BackgroundTilemapEntry* srcMapData
	, int srcMapIndex
	, int size); // signature for all valid template params

template<Fn fn>
static inline void CopyMapWrappedRowToVram(
	GBA::VramAllocator& vram
	, GBA::VramAllocator::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgColStart
	, const GBATEK::BackgroundTilemapEntry* srcMapData
	, u8 mapWidth
	, int srcMapRowToCopy
	, int srcMapColOffset
	, int size
)
{
	int srcMapIndex = mapWidth * srcMapRowToCopy + srcMapColOffset;

	// TODO, this will break if the tilemap is narrower than the render size. Need to guard against this when building tilemap data. 

	if (srcMapColOffset + size > mapWidth)			// Need to wrap the tilemap
	{
		// Draw the row until the edge of the tilemap
		int segment1Size = mapWidth - srcMapColOffset;

		if (segment1Size > 0)
			fn(vram, sbbIndex, destBgRow, destBgColStart, srcMapData, srcMapIndex, segment1Size);

		// Back to the left side of the tilemap
		srcMapIndex -= srcMapColOffset;

		int segment2Size = srcMapColOffset + size - mapWidth;
		if (segment2Size > 0)
			fn(vram, sbbIndex, destBgRow, destBgColStart + segment1Size, srcMapData, srcMapIndex, segment2Size);

	}
	else
	{
		// Freely copy the whole row
		fn(vram, sbbIndex, destBgRow, destBgColStart, srcMapData, srcMapIndex, size);
	}
}

namespace GBA
{
	void Graphics::DrawTilemap
	(
		Tilemap* tilemap
		, const Vector2<FPi16>& position
		, const DrawParams& drawParams
	)
	{
		auto& vram = GBA::VramAllocator::GetInstance();

		const auto tileMapSizeInTiles = tilemap->GetSizeInTiles();

		Vector2<FPi16> newPosition = position;

		// Screen corrections. Final position is the position of the screen on the background.
		newPosition -= drawParams.cameraPosition;								// Convert world space to relative camera space	
		newPosition.x *= -1;
		newPosition += Vector2<FPi16>(tileMapSizeInTiles / 2);		// Offset by map size	
		newPosition *= GBA::Gfx::Tile::PIXELS_SQRROOT_PER_TILE;					// Camera position units to pixel units, 8 pixels per tile/unit
		newPosition -= drawParams.screenSpaceOffset;							// Convert to screen space, position of the screen on the background so it need to be inverted

		Vector2i finalPos{ newPosition.x.ToRoundedInt(), newPosition.y.ToRoundedInt() };

		(*GBATEK::ioRegisterBackgroundScrolls)[tilemap->GetAssignedBackgroundSlot()] = { 
			.xOffset = static_cast<unsigned short>(finalPos.x), 
			.yOffset = static_cast<unsigned short>(finalPos.y) 
		};

		if (tilemap->IsDynamicallyRendered())	// else all the tilemap data should already be loaded
		{
			Vector2i tilemapRenderStartPos = finalPos / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;	// Convert back to tile positions

			// Account for 7 / 8 and -7 / 8 both equalling 0
			if (finalPos.x < 0)
				tilemapRenderStartPos.x -= 1;

			if (finalPos.y < 0)
				tilemapRenderStartPos.y -= 1;

			bool skipRender = false;

			auto& renderData = tilemap->m_renderData;

			if (renderData.lastRenderPosValid)
			{
				Vector2i deltaPos = tilemapRenderStartPos - renderData.lastRenderPos;

				if (deltaPos == Vector2i::Zero)
				{
					skipRender = true;
				}
			}

			if (!skipRender)
			{
				MapWrappingPoints wrappingPoints;
				{
#ifdef PROFILE_RENDER
					PROFILE_SCOPED("Profile Tilemap Renderer dynamic tile cache setup");
#endif
					// Tiles haven't been loaded in, need to plot them in manually for infinite tilemap spoofing
					wrappingPoints = CalculateMapWrappingPoints(tilemapRenderStartPos, drawParams.renderSize, tileMapSizeInTiles, renderData.lastRenderPos, renderData.lastRenderPosValid);
				}

				// "Optimised" tile transferring.
				// tl;dr iterate each row and transfer the blocks of tiles that are viewable. At most 2, start to array end, then array end to wrapped end. 
				auto tileMapEntries = tilemap->GetTileMapEntries();
				auto sbbIndex = tilemap->GetMapScreenBaseBlockIndex();

				auto DrawYRowTiles = [&](int start, int end
					, const MapWrappingPoints::WrappingPointsY& wrapPointsY
					, const MapWrappingPoints::WrappingPointsX& wrapPointsX)
				{
#ifdef LOG_RENDER_ROWCOLS
					DEBUG_LOGFORMAT("[Rendering rows %d through %d", start, end);
#endif
					if (start == end)
						return;

					int tilemapYWrappingOffsetPoint = wrapPointsY.tilemapYWrappingOffsetPoint;

					auto LoopColumns = [&](int destBgRow
						, const GBATEK::BackgroundTilemapEntry* srcMapData
						, u8 mapWidth
						, int srcMapColOffset
						, int size)
					{
						int tileMapYPos = start;
						int y = start;

#define LOOP_THROUGH_ALL_ROWS(TransferMethod) \
for (; y < Math::Min(end, tilemapYWrappingOffsetPoint + 1); ++y)\
{\
	CopyMapWrappedRowToVram<TransferMethod>(vram, sbbIndex, y, destBgRow, srcMapData, mapWidth, tileMapYPos++, srcMapColOffset, size);\
}\
tileMapYPos = 0;\
for (; y < end; ++y)\
{\
	CopyMapWrappedRowToVram<TransferMethod>(vram, sbbIndex, y, destBgRow, srcMapData, mapWidth, tileMapYPos++, srcMapColOffset, size);\
}\
/////
						// Pick the fastest transfer method based on the size of the data we're sending.
						// Numbers are purely based on rough profiling tests
						switch (size)
						{
						case 1:
						{
							LOOP_THROUGH_ALL_ROWS(CopyFromMapToVramSingle);
							break;
						}
						case 2:
						case 3:
						{
							LOOP_THROUGH_ALL_ROWS(CopyFromMapToVramLoop);
							break;
						}
						default:
						{
							LOOP_THROUGH_ALL_ROWS(CopyFromMapToVramMemCpy);
						}
						}
					};
#undef LOOP_THROUGH_ALL_ROWS

					int tileMapSizeInTilesX = tileMapSizeInTiles.x;

					// Slightly faster to have these outside the loop.
					int seg1Size = wrapPointsX.xWrappingOffsetPoint - wrapPointsX.bgTileXStart;
					int bgTileXStart = wrapPointsX.bgTileXStart;
					int tilemapXStart = wrapPointsX.tilemapXStart;
					int bgTileXEnd = wrapPointsX.bgTileXEnd;

#ifdef LOG_RENDER_ROWCOLS
					DEBUG_LOGFORMAT("[Rendering columns %d through %d, 0 through %d", bgTileXStart, bgTileXStart + seg1Size, bgTileXEnd);
#endif

					if (seg1Size > 0)
					{
						LoopColumns(bgTileXStart, tileMapEntries.Data(), tileMapSizeInTilesX, tilemapXStart, seg1Size);
					}

					if (bgTileXEnd > 0)
					{
						LoopColumns(0, tileMapEntries.Data(), tileMapSizeInTilesX, tilemapXStart + seg1Size, bgTileXEnd);
					}
				};
				{
#ifdef PROFILE_RENDER
					PROFILE_SCOPED(Profile Tilemap Renderer dynamic tile load);
#endif
					// Draw onto the GBA bg rows
					if (!renderData.lastRenderPosValid)
					{
						// Cleanly render the full screen
						DrawYRowTiles(
							wrappingPoints.allRow.bgTileYStart,
							wrappingPoints.allRow.yWrappingOffsetPoint,
							wrappingPoints.allRow,
							wrappingPoints.allColumn
						);
					}
					else
					{
						// Draw the new rows (y direction)
						// Do this first as drawing whole row is way more efficient than drawing the columns
						{
							DrawYRowTiles(
								wrappingPoints.newRow.bgTileYStart,
								wrappingPoints.newRow.yWrappingOffsetPoint,
								wrappingPoints.newRow,
								wrappingPoints.allColumn
							);

							DrawYRowTiles(
								0,
								wrappingPoints.newRow.bgTileYEnd,
								wrappingPoints.newRow,
								wrappingPoints.allColumn
							);
						}

						// Draw the new columns (x direction)
						{
							DrawYRowTiles(
								wrappingPoints.allRow.bgTileYStart,
								wrappingPoints.allRow.yWrappingOffsetPoint,
								wrappingPoints.allRow,
								wrappingPoints.newColumn
							);

							DrawYRowTiles(
								0,
								wrappingPoints.allRow.bgTileYEnd,
								wrappingPoints.allRow,
								wrappingPoints.newColumn
							);
						}
					}
				}
			}

			renderData.lastRenderPos = tilemapRenderStartPos;
			renderData.lastRenderPosValid = true;
		}
	}
}
