#include "TilemapRenderer.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/graphicalassets/tilemap/TilemapSet.h"
#include "engine/base/Macros.h"
#include "engine/gba/registers/clock/GBATimer.h"

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
	return m_visible;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/gameobject/Camera.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/screen/Screen.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

void System::TilemapRenderer::VBlankRender(Engine* engine, GameObject* camera)
{
	using namespace GBA;

	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();
	const auto cameraPosition = camera->GetComponent<Component::Transform>()->GetPosition();

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	const Vector2<tFixedPoint8> screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);
	const Vector2<int> screenSizeInTiles = Screen::GetResolution() / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;

	auto* entityManager = engine->GetEntityRegistry();

	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();
	auto& vram = GBA::Vram::GetInstance();

	entityManager->InvokeEach<Component::Transform, Component::TilemapRenderer>(
		[&cameraPosition
		, &screenSpaceOffset
		, &tilemapManager
		, &screenSizeInTiles
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
			profilerClock.SetFrequency(GBA::Timers::Cycle_64);

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

			DEBUG_LOGFORMAT("[Profile Tilemap Renderer positioning] = %d", profilerClock.GetCurrentTimerCount());
			profilerClock.SetActive(false);

			bool dynamicallyLoadTiles = true;
			if (dynamicallyLoadTiles)
			{
				profilerClock.SetActive(true);

				// Tiles haven't been loaded in, need to plot them in manually for infinite tilemap spoofing
				Vector2<int> tilemapRenderStartPos = finalPos / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;	// Convert back to tile positions
				
				// "Optimised" tile transferring. Currently just transfering the whole viewable map per frame, rather than taking into account what's already drawn.
				// tl;dr iterate each row and transfer the blocks of tiles that are viewable. At most 2, start to array end, then array end to wrapped end. 
				const u16* tileMapData = tilemap->GetTileMapData();
				auto sbbIndex = tilemap->GetMapScreenBaseBlockIndex();

				// Cache where we need to wrap indexes. Can't transfer outside tilemap or 32x32 BG range etc, and modulus is expensive in a loop.
				int bgTileXStart = tilemapRenderStartPos.x % TilemapManager::VARIABLE_TILEMAP_SIZE.x;
				int bgTileXEnd = (tilemapRenderStartPos.x + screenSizeInTiles.x + 1) % TilemapManager::VARIABLE_TILEMAP_SIZE.x;
				int xWrappingOffsetPoint = TilemapManager::VARIABLE_TILEMAP_SIZE.x;
				if (bgTileXEnd > bgTileXStart)	// No wrapping occoured
				{
					xWrappingOffsetPoint = bgTileXEnd;
					bgTileXEnd = 0;
				}

				int tilemapXStart = tilemapRenderStartPos.x % tileMapSizeInTiles.x;
				int tilemapXEnd = (tilemapRenderStartPos.x + screenSizeInTiles.x + 1) % tileMapSizeInTiles.x;
				int tilemapXWrappingOffsetPoint = tileMapSizeInTiles.x;
				if (tilemapXEnd > tilemapXStart)	// No wrapping occoured
				{
					tilemapXWrappingOffsetPoint = tilemapXEnd;
					tilemapXEnd = 0;
				}

				int bgTileYStart = tilemapRenderStartPos.y % TilemapManager::VARIABLE_TILEMAP_SIZE.y;
				int bgTileYEnd = (tilemapRenderStartPos.y + screenSizeInTiles.y + 1) % TilemapManager::VARIABLE_TILEMAP_SIZE.y;
				int yWrappingOffsetPoint = TilemapManager::VARIABLE_TILEMAP_SIZE.y;
				if (bgTileYEnd > bgTileYStart)	// No wrapping occoured
				{
					yWrappingOffsetPoint = bgTileYEnd;
					bgTileYEnd = 0;
				}

				int tilemapYStart = tilemapRenderStartPos.y % tileMapSizeInTiles.y;
				int tilemapYEnd = (tilemapRenderStartPos.y + screenSizeInTiles.y + 1) % tileMapSizeInTiles.y;
				int tilemapYWrappingOffsetPoint = tileMapSizeInTiles.y;
				if (tilemapYEnd > tilemapYStart)	// No wrapping occoured
				{
					tilemapYWrappingOffsetPoint = tilemapYEnd;
					tilemapYEnd = 0;
				}

				auto DrawYRowTiles = [&](int start, int end)
				{
					int tileMapYPos = tilemapYStart;

					// Iterating row by row what needs to be drawn
					for (int y = start; y < end; ++y)
					{
						int column = y;
						int tileMapXPos = tilemapXStart;

						// Transfer blocks
						auto DrawXRowTiles = [&](int start, int end)
						{
							int total = end - start;

							if (tileMapXPos + total >= tilemapXWrappingOffsetPoint)
							{
								{
									int delta = tilemapXWrappingOffsetPoint - tileMapXPos;

									int row = start;
									u32 offset = column * TilemapManager::VARIABLE_TILEMAP_SIZE.x + row;

									int tilemapDataIndex = tileMapSizeInTiles.x * tileMapYPos + tileMapXPos;
									vram.SetBackgroundTileData(sbbIndex, offset, &tileMapData[tilemapDataIndex], delta);
								}

								{
									int delta = (tileMapXPos + total) - tilemapXWrappingOffsetPoint;
									tileMapXPos = 0;

									int row = start + (tilemapXWrappingOffsetPoint - tileMapXPos);
									u32 offset = column * TilemapManager::VARIABLE_TILEMAP_SIZE.x + row;

									int tilemapDataIndex = tileMapSizeInTiles.x * tileMapYPos + tileMapXPos;
									vram.SetBackgroundTileData(sbbIndex, offset, &tileMapData[tilemapDataIndex], delta);

									tileMapXPos += delta;
								}
							}
							else
							{
								int row = start;
								u32 offset = column * TilemapManager::VARIABLE_TILEMAP_SIZE.x + row;
								int tilemapDataIndex = tileMapSizeInTiles.x * tileMapYPos + tileMapXPos;

								vram.SetBackgroundTileData(sbbIndex, offset, &tileMapData[tilemapDataIndex], total);

								tileMapXPos += total;
							}
						};

						DrawXRowTiles(bgTileXStart, xWrappingOffsetPoint);
						DrawXRowTiles(0, bgTileXEnd);

						if (++tileMapYPos > tilemapYWrappingOffsetPoint)
						{
							tileMapYPos = 0;
						}
					}
				};

				DrawYRowTiles(bgTileYStart, yWrappingOffsetPoint);
				DrawYRowTiles(0, bgTileYEnd);

				DEBUG_LOGFORMAT("[Profile Tilemap Renderer dynamic tile load = %d", profilerClock.GetCurrentTimerCount());
				profilerClock.SetActive(false);
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
