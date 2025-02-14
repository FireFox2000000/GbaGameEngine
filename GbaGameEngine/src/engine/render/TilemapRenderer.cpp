#include "TilemapRenderer.h"
#include "engine/gba/graphics/tilemap/GBATilemap.h"
#include "engine/base/Macros.h"
#include "engine/graphics/Graphics.h"
#include "gbatek/Backgrounds.h"

void Component::TilemapRenderer::SetTilemap(Tilemap* tilemap)
{
	DEBUG_ASSERTMSG(tilemap->IsLoaded(), "Error: tilemap not loaded");

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
#include "engine/transform/Transform2.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "gbatek/DisplayControl.h"

void System::TilemapRenderer::VBlankRender(GameObject* camera)
{
	const Camera* cameraComponent = camera->GetComponent<Camera>();

	if (cameraComponent->GetProjection() != Camera::Projection::Orthographic)
		return;		// Unhandled, todo

	auto* entityManager = Engine::GetInstance().GetEntityRegistry();

	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();

	const auto drawParams = gfx->CreateDrawParams(camera);

	entityManager->InvokeEach<Transform2, Component::TilemapRenderer>(
		[&drawParams, &gfx]
	(Transform2& transform, Component::TilemapRenderer& tilemapRenderer)
		{
			Tilemap* tilemap = tilemapRenderer.GetTilemap();

			if (!tilemap->IsLoaded())
			{
				DEBUG_ERROR("Cannot render tilemap, has not been loaded.");
				return;
			}

			Vector2<FPi16> position = transform.GetPosition();
			gfx->DrawTilemap(tilemap, position, drawParams);

			// Update extra effects
			if (tilemapRenderer.GetDirty())
			{
				auto& controlRegister = (*GBATEK::ioRegisterBackgroundControls)[tilemap->GetAssignedBackgroundSlot()];

				// SetAffineWrapping
				// SetMosaic
				// SetPriority
				controlRegister.affineWrappingEnabled = tilemapRenderer.GetWrappingEnabled();
				
				GBATEK::ioRegisterDisplayControl->SetBackgroundEnabled(tilemap->GetAssignedBackgroundSlot(), tilemapRenderer.GetVisible());

				tilemapRenderer.ClearDirty();
			}
		});
}
