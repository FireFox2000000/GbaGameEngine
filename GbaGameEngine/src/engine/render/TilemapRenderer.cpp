#include "TilemapRenderer.h"
#include "engine/graphicalassets/tilemap/Tilemap.h"
#include "engine/base/Macros.h"
#include "engine/graphicalassets/tilemap/TilemapManager.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/gameobject/Camera.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gba/registers/display/GBADisplayControl.h"

void System::TilemapRenderer::VBlankRender(Engine* engine, GameObject* camera)
{
	using namespace GBA;

	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	auto* entityManager = engine->GetEntityRegistry();

	TilemapManager* tilemapManager = engine->EditComponent<TilemapManager>();
	auto& vram = GBA::Vram::GetInstance();
	GBA::Graphics* gfx = engine->EditComponent<GBA::Graphics>();

	const auto drawParams = gfx->CreateDrawParams(camera);

	entityManager->InvokeEach<Component::Transform, Component::TilemapRenderer>(
		[&drawParams
		, &tilemapManager
		, &vram
		, &gfx
		]
	(Component::Transform& transform, Component::TilemapRenderer& tilemapRenderer)
		{
			Tilemap* tilemap = tilemapRenderer.GetTilemap();

			if (!tilemap->IsLoaded())
			{
				DEBUG_ASSERTMSG(false, "Cannot render tilemap, has not been loaded.");
				return;
			}

			Vector2<tFixedPoint8> position = transform.GetPosition();
			tilemapRenderer.drawHistory = gfx->DrawTilemap(tilemap, position, drawParams, tilemapRenderer.drawHistory);

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
