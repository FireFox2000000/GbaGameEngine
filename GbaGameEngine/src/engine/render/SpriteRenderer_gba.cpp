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

	if (m_sprite == sprite)
		return;

	if (sprite)
	{
		m_centerToCornerSizeOffset = GBA::AttributeFunctions::GetPixelSize(sprite->GetShape(), sprite->GetSizeMode()) / -2;
	}

	m_sprite = sprite;
}

////////////////////////////////////////////////////////////////////////////////////////////////

#include "engine/gameobject/Camera.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/engine/engine.h"
#include "engine/math/geometry/AxisAlignedBoundingBox.h"

void System::SpriteRenderer::Render(Engine* engine, GameObject* camera)
{
	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();
	const auto cameraPosition = camera->GetComponent<Component::Transform>()->GetPosition();
	DEBUG_LOGFORMAT("Camera Position (%d, %d)", cameraPosition.x.ToRoundedInt(), cameraPosition.y.ToRoundedInt());

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo


	auto* entityManager = engine->GetEntityRegistry();
	GBA::OAMManager* oamManager = engine->EditComponent<GBA::OAMManager>();

	const Vector2<tFixedPoint8> screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);
	AxisAlignedBoundingBox2 orthographicCameraBounds = cameraComponent->GetOrthoBounds();
	orthographicCameraBounds.Translate(cameraPosition);

	entityManager->InvokeEach<Component::Transform, Component::SpriteRenderer>(
		[&oamManager, &cameraPosition, &screenSpaceOffset, &orthographicCameraBounds]
		(Component::Transform& transform, Component::SpriteRenderer& spriteRenderer)
		{
			Sprite* sprite = spriteRenderer.GetSprite();
			if (!sprite)
				return;

			Vector2<tFixedPoint8> position = transform.GetPosition();

			// Frustum culling
			{
				Vector2<tFixedPoint8> worldSpriteSize = GBA::AttributeFunctions::GetTileSize(sprite->GetShape(), sprite->GetSizeMode()) / 2;
				AxisAlignedBoundingBox2 worldSpriteBounds(position - worldSpriteSize, position + worldSpriteSize);

				if (!orthographicCameraBounds.Intersects(worldSpriteBounds))
					return;
			}

			GBA::ObjectAttribute* renderProperties = oamManager->AddToRenderList(sprite);

			Vector2<tFixedPoint8> newPosition = position;
			newPosition -= cameraPosition;											// Convert world space to relative camera space	
			newPosition.y *= -1;														// Correct for screen space starting from the top
			newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			newPosition += screenSpaceOffset;											// Convert to screen space
			newPosition += spriteRenderer.GetCenterToCornerSizeOffset();				// Offset by sprite size to render from the center
			
			renderProperties->SetPriority(GBA::Attributes::Layer2);

			Component::Transform::tScale scale = transform.GetScale();
			renderProperties->SetPosition(newPosition);
			renderProperties->SetFlippedHorizontal((int)scale.x < 0);
			renderProperties->SetFlippedVertical((int)scale.y < 0);
		});
}
