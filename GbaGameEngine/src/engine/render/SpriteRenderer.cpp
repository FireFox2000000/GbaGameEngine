#include "SpriteRenderer.h"

#include "engine/base/Macros.h"
#include "engine/base/core/stl/FixedPoint.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/screen/Screen.h"

void Component::SpriteRenderer::SetSprite(Sprite* sprite)
{
	if (m_sprite == sprite)
		return;

	if (sprite)
	{
		m_centerToCornerSizeOffset = sprite->GetSizeInPixels() / -2;
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

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo


	auto* entityManager = engine->GetEntityRegistry();
	Graphics* gfx = engine->EditComponent<Graphics>();

	const auto drawParams = gfx->CreateDrawParams(camera);

	AxisAlignedBoundingBox2 orthographicCameraBounds = cameraComponent->GetOrthoBounds();
	orthographicCameraBounds.Translate(drawParams.cameraPosition);

	entityManager->InvokeEach<Component::Transform, Component::SpriteRenderer>(
		[&gfx, &drawParams, &orthographicCameraBounds]
		(Component::Transform& transform, Component::SpriteRenderer& spriteRenderer)
		{
			Sprite* sprite = spriteRenderer.GetSprite();
			if (!sprite)
				return;

			Vector2<tFixedPoint8> position = transform.GetPosition();
			Vector2<tFixedPoint8> scale = transform.GetScale();
			tFixedPoint8 rotation = transform.GetRotationDegrees();
			
			bool hasAffineTransformation = transform.HasAffineTransformation();

			// Frustum culling
			{
				Vector2<tFixedPoint8> worldSpriteSize = sprite->GetSize();
				if (hasAffineTransformation)	// if we have affine transformation then this will compensate for ObjAffineDoubleRendering/rotation going outside sprite bounds
				{
					worldSpriteSize *= 2;
				}

				AxisAlignedBoundingBox2 worldSpriteBounds(position - worldSpriteSize, position + worldSpriteSize);

				if (!orthographicCameraBounds.Intersects(worldSpriteBounds))
					return;
			}
			
			gfx->DrawSprite(
				sprite,
				position,
				scale,
				rotation,
				spriteRenderer.GetCenterToCornerSizeOffset(), 
				hasAffineTransformation,
				drawParams
			);
		});
}
