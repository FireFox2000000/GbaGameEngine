#include "PlayerPrefab.h"
#include "engine/engine/engine.h"
#include "engine/asset/libraries/SpriteLibrary.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/asset/libraries/AnimationLibrary.h"
#include "engine/gameobject/GameObject.h"
#include "engine/animation/SpriteAnimator.h"

#include "game/scripts/componentsystems/movement/RpgMovement.h"
#include "game/scripts/componentsystems/PlayerComponent.h"
#include "game/scripts/componentsystems/collision/Collider.h"
#include "game/scripts/componentsystems/interaction/RpgInteraction.h"

void PlayerPrefab::MakePlayerObj(Engine * engine, GameObject & out_go)
{
	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();
	AnimationLibrary* animLibrary = engine->EditComponent<AnimationLibrary>();

	Component::SpriteRenderer& testBackgroundRenderer = out_go.AddComponent<Component::SpriteRenderer>();
	Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Reimu, 0);
	testBackgroundRenderer.SetSprite(shantae0);

	out_go.AddComponent<Component::Player>();

	Vector2<tFixedPoint8> halfSize = Vector2<tFixedPoint8>(2, 2) / 2;

	{
		Component::RpgMovement& movement = out_go.AddComponent<Component::RpgMovement>();
		movement.speed = 10;
		movement.enableHorizontalDirSpriteFlip = true;
		movement.SetCurrentDirection(Component::RpgMovement::None);

		movement.idleAnimations[Component::RpgMovement::Down] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuIdleDown);
		movement.idleAnimations[Component::RpgMovement::Up] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuIdleUp);
		movement.idleAnimations[Component::RpgMovement::Left] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuIdleLeft);
		movement.idleAnimations[Component::RpgMovement::Right] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuIdleLeft);

		movement.movementAnimations[Component::RpgMovement::Down] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuWalkDown);
		movement.movementAnimations[Component::RpgMovement::Up] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuWalkUp);
		movement.movementAnimations[Component::RpgMovement::Left] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuWalkLeft);
		movement.movementAnimations[Component::RpgMovement::Right] = animLibrary->GetSpriteAnimation(SpriteAnimationID::ReimuWalkLeft);
	}

	Vector2<tFixedPoint8> collisionOffset = Vector2<tFixedPoint8>(0, -1.f);

	{
		Component::Collider& collider = out_go.AddComponent<Component::Collider>();

		Vector2<tFixedPoint8> min = Vector2<tFixedPoint8>(-1 * halfSize + collisionOffset);
		Vector2<tFixedPoint8> max = Vector2<tFixedPoint8>(halfSize + collisionOffset);
		collider.shape = AxisAlignedBoundingBox2(min, max);
	}

	{
		Component::RpgInteractor& interactor = out_go.AddComponent<Component::RpgInteractor>();
		auto interactionTriggerSize = halfSize / 2;
		Vector2<tFixedPoint8> min = Vector2<tFixedPoint8>(-1 * interactionTriggerSize + collisionOffset);
		Vector2<tFixedPoint8> max = Vector2<tFixedPoint8>(interactionTriggerSize + collisionOffset);

		interactor.interactionTrigger = AxisAlignedBoundingBox2(min, max);
		interactor.facingProjectionOffset = Vector2<tFixedPoint8>(halfSize.x, halfSize.y);
	}

	out_go.AddComponent<Component::SpriteAnimator>();
}
