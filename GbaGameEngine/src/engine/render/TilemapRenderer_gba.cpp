#include "TilemapRenderer.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/graphicalassets/tilemap/TilemapSet.h"
#include "engine/base/Macros.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

Component::TilemapRenderer::~TilemapRenderer()
{
	FreeAssignedBackgroundSlot();
}

void Component::TilemapRenderer::SetTilemap(Tilemap* tilemap)
{
	m_tilemap = tilemap;
	SetDirty();
}

bool Component::TilemapRenderer::AssignBackgroundSlot()
{
	DEBUG_ASSERTMSG(!IsBackgroundSlotAssigned(), "Trying to assign a new background slot when it already has one");

	m_backgroundSlotId = GBA::BackgroundControl::ReserveBackground();
	SetDirty();

	return m_backgroundSlotId < GBA::BackgroundControl::Count;
}

void Component::TilemapRenderer::FreeAssignedBackgroundSlot()
{
	GBA::BackgroundControl::FreeBackground(m_backgroundSlotId);
	m_backgroundSlotId = GBA::BackgroundControl::Count;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/gameobject/Camera.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/screen/Screen.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"

void System::TilemapRenderer::VBlankRender(Engine* engine, GameObject* camera)
{
	using namespace GBA;

	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();
	const auto cameraPosition = camera->GetComponent<Component::Transform>()->position;

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	const Vector2<tFixedPoint8> screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);

	auto* entityManager = engine->GetEntityRegistry();

	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();

	entityManager->InvokeEach<Component::Transform, Component::TilemapRenderer>(
		[&cameraPosition, &screenSpaceOffset, &tilemapManager]
	(Component::Transform& transform, Component::TilemapRenderer& tilemapRenderer)
		{
			Tilemap* tilemap = tilemapRenderer.GetTilemap();
			if (!tilemap)
			{
				if (tilemapRenderer.IsBackgroundSlotAssigned())
				{
					tilemapRenderer.FreeAssignedBackgroundSlot();
				}

				return;
			}

			// Register ourselves
			if (!tilemapRenderer.IsBackgroundSlotAssigned())
			{
				tilemapRenderer.AssignBackgroundSlot();
			}

			// TODO, need to handle tilemap unloading somehow
			if (!tilemap->IsLoaded())
			{
				tilemapManager->Load(*tilemap);
			}

			Vector2<tFixedPoint8> position = transform.position;
			
			// Screen corrections. Final position is the position of the screen on the background.
			position -= cameraPosition;											// Convert world space to relative camera space	
			position.x *= -1;
			position += tilemap->GetSizeInTiles() / 2;								// Offset by map size	
			position *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			position -= screenSpaceOffset;											// Convert to screen space, position of the screen on the background so it need to be inverted

			auto& background = BackgroundControl::GetBackground(tilemapRenderer.GetAssignedBackgroundSlot());
			background.SetPosition(position);

			if (tilemapRenderer.GetDirty())
			{
				TilemapSet* tilemapSet = tilemap->EditTilemapSet();
				Background::ColourMode colourMode = Background::GetColourModeFromCompression(tilemapSet->m_tileSetDataCompressionFlags);
				background.SetColourMode(colourMode);
				background.SetCharacterBaseBlock(tilemapSet->GetTileSetCharacterBaseBlock());
				background.SetScreenBaseBlock(tilemap->GetMapScreenBaseBlockIndex());
				background.SetSize(tilemap->GetSize());

				tilemapRenderer.ClearDirty();
			}
		});
}
