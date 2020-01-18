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

	{
		Component::Collider& collider = out_go.AddComponent<Component::Collider>();
		collider.shape = AxisAlignedBoundingBox2(halfSize * -1, halfSize);
	}

	{
		Component::RpgInteractor& interactor = out_go.AddComponent<Component::RpgInteractor>();
		auto interactionTriggerSize = halfSize / 2;
		interactor.interactionTrigger = AxisAlignedBoundingBox2(interactionTriggerSize * -1, interactionTriggerSize);
		interactor.facingProjectionOffset = Vector2<tFixedPoint8>(halfSize.x, halfSize.y);
	}

	out_go.AddComponent<Component::SpriteAnimator>();
}
