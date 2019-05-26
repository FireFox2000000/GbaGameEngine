#include "SpriteRenderer.h"

#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/base/Macros.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/engine/engine.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "engine/graphicalassets/tile/Tile.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"
#include "engine/screen/Screen.h"

void Component::SpriteRenderer::SetSprite(Sprite* sprite)
{
	using namespace GBA::Attributes;

	if (sprite)
	{
		m_centerToCornerSizeOffset = GBA::AttributeFunctions::GetPixelSize(sprite->GetShape(), sprite->GetSizeMode()) / -2;
	}

	m_sprite = sprite;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/gameobject/Camera.h"
#include "engine/gameobject/transformation/Position.h"
#include "engine/engine/engine.h"

void System::SpriteRenderer::Render(Engine* engine, GameObject* camera)
{
	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();
	const Component::Position* cameraPosition = camera->GetComponent<Component::Position>();

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	auto* entityManager = engine->GetEntityRegistry();
	GBA::OAMManager* oamManager = engine->GetOAMManager();

	entityManager->InvokeEach<Component::Position, Component::SpriteRenderer>([&oamManager, &cameraPosition](Component::Position& position, Component::SpriteRenderer& spriteRenderer)
		{
			if (!spriteRenderer.GetSprite())
				return;

			GBA::OAMSpriteRenderProperties* renderProperties = oamManager->AddToRenderList(spriteRenderer.GetSprite());

			Vector2<FixedPoint<int, 8> > newPosition = position;
			newPosition -= *cameraPosition;											// Convert world space to relative camera space	
			newPosition.y *= -1;														// Correct for screen space starting from the top
			newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			newPosition += Screen::GetResolution() / FixedPoint<int, 8>(2);			// Convert to screen space
			newPosition += spriteRenderer.GetCenterToCornerSizeOffset();				// Offset by sprite size to render from the center

			renderProperties->oamProperties.SetPosition(newPosition);
		});
}
