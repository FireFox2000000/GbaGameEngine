#include "DebugRender.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"
#include "engine/graphicalassets/Graphics.h"
#include "engine/screen/Screen.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/physics/Collider.h"

#include "game/data/sprites/debug/Debug_Primitives_64x64.h"

DebugRender::DebugRender()
{
	m_assetManager.AddSpriteSheetFromFile(DebugPrimitives, Debug_Primitives_64x64::data);
}

void DebugRender::RenderColliders(Engine* engine, const GameObject* camera)
{
	// Debug sprites are set to 64x64 in order to be able to render over a large variety of sizes. Scaling up has limits, scaling down does not (in general).
	// CHange this if the size of the debug primitives change as well. 
	constexpr float SpriteScaleTo1x1 = 1.f / 4.f;

	const Component::Camera* cameraComponent = camera->GetComponent<Component::Camera>();

	if (cameraComponent->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo


	auto* entityManager = engine->GetEntityRegistry();
	Graphics* gfx = engine->GetComponent<Graphics>();

	const auto drawParams = gfx->CreateDrawParams(camera);

	AxisAlignedBoundingBox2 orthographicCameraBounds = cameraComponent->GetOrthoBounds();
	orthographicCameraBounds.Translate(drawParams.cameraPosition);

	entityManager->InvokeEach<Component::Transform, Component::Collider>(
		[this, &gfx, &drawParams, &orthographicCameraBounds]
	(Component::Transform& transform, Component::Collider& collider)
		{
			Sprite* sprite = nullptr;

			Vector2<tFixedPoint8> position = transform.GetPosition();

			constexpr tFixedPoint24 BaseScale = SpriteScaleTo1x1;
			Vector2<tFixedPoint24> scale = Vector2<tFixedPoint24>(BaseScale, BaseScale);

			tFixedPoint8 rotation = 0;

			switch (collider.GetShapeType())
			{
			case ColliderShapeType::AABB:
			{
				sprite = m_assetManager.GetSprite(DebugPrimitives, HollowSquare);

				auto aabb = collider.GetAABB();
				auto transformScale = transform.GetScale();

				aabb.min.x = tFixedPoint8(tFixedPoint24(aabb.min.x) * transformScale.x);
				aabb.min.y = tFixedPoint8(tFixedPoint24(aabb.min.y) * transformScale.y);
				aabb.max.x = tFixedPoint8(tFixedPoint24(aabb.max.x) * transformScale.x);
				aabb.max.y = tFixedPoint8(tFixedPoint24(aabb.max.y) * transformScale.y);

				tFixedPoint8 halfExtentX = tFixedPoint8(0.5f) * (aabb.max.x - aabb.min.x);
				tFixedPoint8 halfExtentY = tFixedPoint8(0.5f) * (aabb.max.y - aabb.min.y);

				scale.x *= tFixedPoint24(halfExtentX);
				scale.y *= tFixedPoint24(halfExtentY);

				position += aabb.min + Vector2<tFixedPoint8>(halfExtentX, halfExtentY);

				break;
			}
			case ColliderShapeType::Circle:
			{
				sprite = m_assetManager.GetSprite(DebugPrimitives, HollowCircle);

				const auto& circle = collider.GetCircle();
				scale.x *= tFixedPoint24(circle.radius);
				scale.y *= tFixedPoint24(circle.radius);

				break;
			}
			default: return;	// Unhandled
			}

			bool hasAffineTransformation = true;

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

			Vector2<int> centerToCornerSizeOffset = sprite->GetSizeInPixels() / -2;

			gfx->DrawSprite(
				sprite,
				position,
				scale,
				rotation,
				centerToCornerSizeOffset,
				hasAffineTransformation,
				drawParams
			);
		});
}
