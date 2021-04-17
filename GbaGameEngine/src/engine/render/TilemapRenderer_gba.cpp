#include "TilemapRenderer.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/graphicalassets/tilemap/TilemapSet.h"
#include "engine/base/Macros.h"
#include "engine/gba/registers/clock/GBATimer.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"

//#define LOG_RENDER_ROWCOLS

void Component::TilemapRenderer::SetTilemap(Tilemap* tilemap)
{
	m_tilemap = tilemap;
	SetDirty();
}

void Component::TilemapRenderer::SetWrappingEnabled(bool enabled)
{
	if (m_wrapping != enabled)
	{
		m_wrapping = enabled;
		SetDirty();
	}
}

bool Component::TilemapRenderer::GetWrappingEnabled() const
{
	return m_wrapping;
}

void Component::TilemapRenderer::SetVisible(bool enabled)
{
	if (m_visible != enabled)
	{
		m_visible = enabled;
		SetDirty();
	}
}

bool Component::TilemapRenderer::GetVisible() const
{  
#ifdef RETAIL 
	asd
#endif
	return m_visible;
}

Component::TilemapRenderer::MapWrappingPoints Component::TilemapRenderer::CalculateMapWrappingPoints(
	const Vector2<int>& tilemapRenderStartPos
	, const Vector2<int>& renderSize
	, const Vector2<u8>& tileMapSizeInTiles
)
{
	using namespace GBA;
	Vector2<int> deltaPos = tilemapRenderStartPos - m_lastRenderPos;

	// Cache where we need to wrap indexes. Can't transfer outside tilemap or 32x32 BG range etc, and modulus is expensive in a loop.
	int bgTileXStartAll = 0, bgTileXEndAll = 0;
	int bgTileXStartNew = 0, bgTileXEndNew = 0;

	bgTileXStartAll = Math::Power2Mod(tilemapRenderStartPos.x, TilemapManager::VARIABLE_TILEMAP_SIZE.x);
	bgTileXEndAll = Math::Power2Mod((tilemapRenderStartPos.x + renderSize.x), TilemapManager::VARIABLE_TILEMAP_SIZE.x);

	if (m_lastRenderPosValid && deltaPos.x != 0)
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
		tilemapXStartAll = Math::Mod(tilemapRenderStartPos.x, tileMapSizeInTiles.x);
	}
	if (m_lastRenderPosValid && deltaPos.x > 0)
	{
		// Only render the new tiles on the screen
		tilemapXStartNew = Math::Mod(tilemapRenderStartPos.x + renderSize.x - deltaPos.x, tileMapSizeInTiles.x);
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

	if (m_lastRenderPosValid && deltaPos.y != 0)
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

	if (m_lastRenderPosValid && deltaPos.x == 0)
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
		tilemapYStartAll = Math::Mod(tilemapRenderStartPos.y, tileMapSizeInTiles.y);
	}
	if (m_lastRenderPosValid && deltaPos.y > 0)
	{
		// Only render the new tiles on the screen
		tilemapYStartNew = Math::Mod(tilemapRenderStartPos.y + renderSize.y - deltaPos.y, tileMapSizeInTiles.y);
	}
	else
	{
		tilemapYStartNew = tilemapYStartAll;
	}

	int tilemapYEndAll = Math::Mod((tilemapRenderStartPos.y + renderSize.y), tileMapSizeInTiles.y);
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

	if (m_lastRenderPosValid && deltaPos.y == 0)
	{
		bgTileYStartNew = 0;
		bgTileYEndNew = 0;
		yWrappingOffsetPointNew = 0;
	}

	if (m_lastRenderPosValid && deltaPos.x == 0)
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

////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/gameobject/Camera.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/screen/Screen.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

static inline void CopyFromMapToVram(
	GBA::Vram& vram
	, GBA::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgCol
	, const u16* srcMapData
	, int srcMapIndex
	, int size
	)
{
	u32 offset = destBgRow * TilemapManager::VARIABLE_TILEMAP_SIZE.x + destBgCol;
	if (size == 1)
	{
		for (int i = 0; i < size; ++i)
		{
			vram.SetBackgroundTileData(sbbIndex, offset + i, srcMapData[srcMapIndex + i]);
		}
	}
	else
	{
		vram.SetBackgroundTileData(sbbIndex, offset, &srcMapData[srcMapIndex], size);
	}	
}

static inline void CopyMapWrappedRowToVram(
	GBA::Vram& vram
	, GBA::tScreenBaseBlockIndex sbbIndex
	, int destBgRow
	, int destBgColStart
	, const u16* srcMapData
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
			CopyFromMapToVram(vram, sbbIndex, destBgRow, destBgColStart, srcMapData, srcMapIndex, segment1Size);

		// Back to the left side of the tilemap
		srcMapIndex -= srcMapColOffset;

		int segment2Size = srcMapColOffset + size - mapWidth;
		if (segment2Size > 0)
			CopyFromMapToVram(vram, sbbIndex, destBgRow, destBgColStart + segment1Size, srcMapData, srcMapIndex, segment2Size);
		
	}
	else
	{
		// Freely copy the whole row
		CopyFromMapToVram(vram, sbbIndex, destBgRow, destBgColStart, srcMapData, srcMapIndex, size);
	}
}

void System::TilemapRenderer::VBlankRender(Engine* engine, GameObject* camera)
{
	using namespace GBA;

	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();
	const auto cameraPosition = camera->GetComponent<Component::Transform>()->GetPosition();

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	const Vector2<tFixedPoint8> screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);
	const Vector2<int> screenSizeInTiles = Screen::GetResolution() / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
	const Vector2<int> renderSize = screenSizeInTiles + Vector2<int>(1, 1);

	auto* entityManager = engine->GetEntityRegistry();

	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();
	auto& vram = GBA::Vram::GetInstance();

	entityManager->InvokeEach<Component::Transform, Component::TilemapRenderer>(
		[&cameraPosition
		, &screenSpaceOffset
		, &tilemapManager
		, &renderSize
		, &vram
		]
	(Component::Transform& transform, Component::TilemapRenderer& tilemapRenderer)
		{
			Tilemap* tilemap = tilemapRenderer.GetTilemap();

			if (!tilemap->IsLoaded())
			{
				DEBUG_ASSERTMSG(false, "Cannot render tilemap, has not been loaded.");
				return;
			}

			auto& profilerClock = GBA::Timers::GetTimer(GBA::Timers::Profile);
			profilerClock.SetFrequency(GBA::Timers::Cycle_1);

			profilerClock.SetActive(true);

			Vector2<tFixedPoint8> position = transform.GetPosition();
			const auto tileMapSizeInTiles = tilemap->GetSizeInTiles();
			
			// Screen corrections. Final position is the position of the screen on the background.
			position -= cameraPosition;											// Convert world space to relative camera space	
			position.x *= -1;
			position += tileMapSizeInTiles / 2;								// Offset by map size	
			position *= GBA::Gfx::Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			position -= screenSpaceOffset;											// Convert to screen space, position of the screen on the background so it need to be inverted

			Vector2<int> finalPos(position.x.ToRoundedInt(), position.y.ToRoundedInt());
			BackgroundControl::SetBackgroundScrollingPosition(tilemap->GetAssignedBackgroundSlot(), finalPos.x, finalPos.y);

			//DEBUG_LOGFORMAT("[Profile Tilemap Renderer positioning] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);

			if (tilemap->IsDynamicallyRendered())	// else all the tilemap data should already be loaded
			{
				Vector2<int> tilemapRenderStartPos = finalPos / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;	// Convert back to tile positions

				// Account for 7 / 8 and -7 / 8 both equalling 0
				if (finalPos.x < 0)
					tilemapRenderStartPos.x -= 1;

				if (finalPos.y < 0)
					tilemapRenderStartPos.y -= 1;

				bool skipRender = false;
				if (tilemapRenderer.m_lastRenderPosValid)
				{
					Vector2<int> deltaPos = tilemapRenderStartPos - tilemapRenderer.m_lastRenderPos;

					if (deltaPos == Vector2<int>::Zero)
					{
						skipRender = true;
					}
				}

				if (!skipRender)
				{
					profilerClock.SetActive(true);

					// Tiles haven't been loaded in, need to plot them in manually for infinite tilemap spoofing
					Component::TilemapRenderer::MapWrappingPoints wrappingPoints = tilemapRenderer.CalculateMapWrappingPoints(tilemapRenderStartPos, renderSize, tileMapSizeInTiles);

					DEBUG_LOGFORMAT("[Profile Tilemap Renderer dynamic tile cache setup = %d", profilerClock.GetCurrentTimerCount());
					profilerClock.SetActive(false);

					// "Optimised" tile transferring.
					// tl;dr iterate each row and transfer the blocks of tiles that are viewable. At most 2, start to array end, then array end to wrapped end. 
					const u16* tileMapData = tilemap->GetTileMapData();
					auto sbbIndex = tilemap->GetMapScreenBaseBlockIndex();	

					auto DrawYRowTiles = [&](int start, int end
						, const Component::TilemapRenderer::MapWrappingPoints::WrappingPointsY& wrapPointsY
						, const Component::TilemapRenderer::MapWrappingPoints::WrappingPointsX& wrapPointsX)
					{
#ifdef LOG_RENDER_ROWCOLS
						DEBUG_LOGFORMAT("[Rendering rows %d through %d", start, end);
#endif
						if (start == end)
							return;

						int tileMapYPos = start;
						int tileMapSizeInTilesX = tileMapSizeInTiles.x;

						// Slightly faster to have these outside the loop.
						int seg1Size = wrapPointsX.xWrappingOffsetPoint - wrapPointsX.bgTileXStart;
						int bgTileXStart = wrapPointsX.bgTileXStart;
						int tilemapXStart = wrapPointsX.tilemapXStart;
						int bgTileXEnd = wrapPointsX.bgTileXEnd;
						int tilemapYWrappingOffsetPoint = wrapPointsY.tilemapYWrappingOffsetPoint;

#ifdef LOG_RENDER_ROWCOLS
						DEBUG_LOGFORMAT("[Rendering columns %d through %d, 0 through %d", bgTileXStart, bgTileXStart + seg1Size, bgTileXEnd);
#endif
						// Iterating row by row what needs to be drawn
						for (int y = start; y < end; ++y)
						{
							// Wrap around the background object itself
							CopyMapWrappedRowToVram(vram, sbbIndex, y, bgTileXStart, tileMapData, tileMapSizeInTilesX, tileMapYPos, tilemapXStart, seg1Size);
							CopyMapWrappedRowToVram(vram, sbbIndex, y, 0, tileMapData, tileMapSizeInTilesX, tileMapYPos, tilemapXStart + seg1Size, bgTileXEnd);

							if (++tileMapYPos > tilemapYWrappingOffsetPoint)
							{
								tileMapYPos = 0;
							}
						}
					};

					profilerClock.SetActive(true);

					// Draw onto the GBA bg rows
					if (!tilemapRenderer.m_lastRenderPosValid)
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

						// Draw the new rows (y direction)
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
					}

					DEBUG_LOGFORMAT("[Profile Tilemap Renderer dynamic tile load = %d", profilerClock.GetCurrentTimerCount());
					profilerClock.SetActive(false);
				}

				// Track what's already been rendered so we can skip it next frame
				tilemapRenderer.m_lastRenderPos = tilemapRenderStartPos;
				tilemapRenderer.m_lastRenderPosValid = true;
			}

			// Update extra effects
			if (tilemapRenderer.GetDirty())
			{
				auto& controlRegister = BackgroundControl::GetBgControlRegister(tilemap->GetAssignedBackgroundSlot());

				// SetAffineWrapping
				// SetMosaic
				// SetPriority
				controlRegister.SetAffineWrapping(tilemapRenderer.GetWrappingEnabled());
				DisplayControl::SetBackgroundActive(tilemap->GetAssignedBackgroundSlot(), tilemapRenderer.GetVisible());

				tilemapRenderer.ClearDirty();
			}
		});
}
