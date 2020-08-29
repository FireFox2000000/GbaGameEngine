#include "TilemapRenderer.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/graphicalassets/tilemap/TilemapSet.h"
#include "engine/base/Macros.h"

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

	auto* entityManager = engine->GetEntityRegistry();

	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();

	entityManager->InvokeEach<Component::Transform, Component::TilemapRenderer>(
		[&cameraPosition, &screenSpaceOffset, &tilemapManager]
	(Component::Transform& transform, Component::TilemapRenderer& tilemapRenderer)
		{
			Tilemap* tilemap = tilemapRenderer.GetTilemap();

			DEBUG_ASSERTMSG(tilemap->IsLoaded(), "Cannot render tilemap, has not been loaded.");

			// TODO, need to handle tilemap unloading somehow
			if (!tilemap->IsLoaded())
			{
				return;
			}

			Vector2<tFixedPoint8> position = transform.GetPosition();
			
			// Screen corrections. Final position is the position of the screen on the background.
			position -= cameraPosition;											// Convert world space to relative camera space	
			position *= -1;
			position += tilemap->GetSizeInTiles() / 2;								// Offset by map size	
			position *= GBA::Gfx::Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			position -= screenSpaceOffset;											// Convert to screen space, position of the screen on the background so it need to be inverted

			BackgroundControl::SetBackgroundScrollingPosition(tilemap->GetAssignedBackgroundSlot(), position.x.ToRoundedInt(), position.y.ToRoundedInt());

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
