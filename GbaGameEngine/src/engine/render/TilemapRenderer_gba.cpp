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
			const auto sizeInTiles = tilemap->GetSizeInTiles();
			
			// Screen corrections. Final position is the position of the screen on the background.
			position -= cameraPosition;											// Convert world space to relative camera space	
			position.x *= -1;
			position += sizeInTiles / 2;								// Offset by map size	
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
				
				// TODO, optimise for what's already loaded in, just brute forcing for now. Way too slow though.
				const u16* tileMapData = tilemap->GetTileMapData();
				auto sbbIndex = tilemap->GetMapScreenBaseBlockIndex();

				for (int y = 0; y < screenSizeInTiles.y + 1; ++y)
				{
					int yPos = (tilemapRenderStartPos.y + y);
					int tileMapYPos = yPos % sizeInTiles.y;
					int column = yPos % TilemapManager::VARIABLE_TILEMAP_SIZE.y;

					// Cache for wrapping index, removes expensive and unnessacary modulus calls
					int tileMapWrapOffsetX = 0;		
					int vTileRowWrapOffsetX = 0;

					// Loop through and plot tiles
					for (int x = 0; x < screenSizeInTiles.x + 1; ++x)
					{
						// If we try to render beyond the bounds of the tilemap, render it like it's wrapped around
						int xPos = (tilemapRenderStartPos.x + x);

						int tileMapXPos = xPos - tileMapWrapOffsetX;
						while (tileMapXPos >= sizeInTiles.x)
						{
							tileMapWrapOffsetX += sizeInTiles.x;
							tileMapXPos -= sizeInTiles.x;
						}

						int tilemapDataIndex = sizeInTiles.x * tileMapYPos + tileMapXPos;
						u16 tileInfo = tileMapData[tilemapDataIndex];

						// Load the tile information into VRAM
						{
							int row = xPos - vTileRowWrapOffsetX;
							while (row >= TilemapManager::VARIABLE_TILEMAP_SIZE.x)
							{
								vTileRowWrapOffsetX += TilemapManager::VARIABLE_TILEMAP_SIZE.x;
								row -= TilemapManager::VARIABLE_TILEMAP_SIZE.x;
							}

							u32 offset = column * TilemapManager::VARIABLE_TILEMAP_SIZE.x + row;
							vram.SetBackgroundTileData(sbbIndex, offset, tileInfo);
						}
					}
				}

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
