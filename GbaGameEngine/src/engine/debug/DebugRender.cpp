#include "DebugRender.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/screen/Screen.h"
#include "engine/transform/Transform2.h"
#include "engine/physics/Collider.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlas.h"
#include "engine/gba/graphics/sprite/GBASpriteNode.h"

#include "game/data/sprites/debug/Debug_Primitives_64x64.h"

DebugRender::DebugRender()
{
	m_spritePrimitives = Engine::GetInstance().GetResourcesManager().LoadSpriteAtlas(Debug_Primitives_64x64::data);
}

DebugRender::~DebugRender()
{
	Engine::GetInstance().GetResourcesManager().Unload(m_spritePrimitives);
}

void DebugRender::RenderColliders(const GameObject* camera)
{
	// Debug sprites are set to 64x64 in order to be able to render over a large variety of sizes. Scaling up has limits, scaling down does not (in general).
	// CHange this if the size of the debug primitives change as well. 
	constexpr float SpriteScaleTo1x1 = 1.f / 4.f;

	const Camera* cameraComponent = camera->GetComponent<Camera>();

	if (cameraComponent->GetProjection() != Camera::Projection::Orthographic)
		return;		// Unhandled, todo


	auto* entityManager = Engine::GetInstance().GetEntityRegistry();
	Graphics* gfx = Engine::GetInstance().GetComponent<Graphics>();

	const auto drawParams = gfx->CreateDrawParams(camera);

	AxisAlignedBoundingBox2 orthographicCameraBounds = cameraComponent->GetOrthoBounds();
	orthographicCameraBounds.Translate(drawParams.cameraPosition);

	entityManager->InvokeEach<Transform2, Collider>(
		[this, &gfx, &drawParams, &orthographicCameraBounds]
	(Transform2& transform, Collider& collider)
		{
			GBA::Gfx::Sprite* sprite = nullptr;

			Vector2<FPi16> position = transform.GetPosition();

			constexpr FPi16 BaseScale = SpriteScaleTo1x1;
			Vector2<FPi16> scale { BaseScale, BaseScale };

			FPi16 rotation = 0;

			switch (collider.GetShapeType())
			{
			case ColliderShapeType::AABB:
			{
				sprite = m_spritePrimitives->GetSprite(0);

				auto aabb = collider.GetAABB();
				auto transformScale = transform.GetScale();

				aabb.min.x *= transformScale.x;
				aabb.min.y *= transformScale.y;
				aabb.max.x *= transformScale.x;
				aabb.max.y *= transformScale.y;

				FPi16 halfExtentX = FPi16(0.5f) * (aabb.max.x - aabb.min.x);
				FPi16 halfExtentY = FPi16(0.5f) * (aabb.max.y - aabb.min.y);

				scale.x *= FPi16(halfExtentX);
				scale.y *= FPi16(halfExtentY);

				position += aabb.min + Vector2<FPi16>{ halfExtentX, halfExtentY };

				break;
			}
			case ColliderShapeType::Circle:
			{
				sprite = m_spritePrimitives->GetSprite(1);

				const auto& circle = collider.GetCircle();
				scale.x *= FPi16(circle.radius);
				scale.y *= FPi16(circle.radius);

				break;
			}
			default: return;	// Unhandled
			}

			bool hasAffineTransformation = Transform2::HasAffineTransformation(position, scale, rotation);

			// Frustum culling
			{
				Vector2<FPi16> worldSpriteSize = Vector2<FPi16>(sprite->GetSize());
				if (hasAffineTransformation)	// if we have affine transformation then this will compensate for ObjAffineDoubleRendering/rotation going outside sprite bounds
				{
					worldSpriteSize *= 2;
				}

				AxisAlignedBoundingBox2 worldSpriteBounds(position - worldSpriteSize, position + worldSpriteSize);

				if (!orthographicCameraBounds.Intersects(worldSpriteBounds))
					return;
			}

			Vector2i centerToCornerSizeOffset = sprite->GetSizeInPixels() / -2;

			gfx->DrawSprite(
				sprite,
				position,
				scale,
				rotation,
				centerToCornerSizeOffset,
				drawParams
			);
		});
}
