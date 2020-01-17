#include "PlayerPrefab.h"
#include "engine/engine/engine.h"
#include "engine/asset/libraries/SpriteLibrary.h"
#include "engine/render/SpriteRenderer.h"
#include "engine/asset/libraries/SpriteLibrary.h"
#include "engine/asset/libraries/AnimationLibrary.h"
#include "engine/gameobject/GameObject.h"
#include "engine/animation/SpriteAnimator.h"

#include "game/scripts/componentsystems/movement/RpgMovement.h"
#include "game/scripts/componentsystems/PlayerComponent.h"
#include "game/scripts/componentsystems/collision/Collider.h"

void PlayerPrefab::MakePlayerObj(Engine * engine, GameObject & out_go)
{
	SpriteLibrary* spriteLibrary = engine->EditComponent<SpriteLibrary>();

	Component::SpriteRenderer& testBackgroundRenderer = out_go.AddComponent<Component::SpriteRenderer>();
	Sprite* shantae0 = spriteLibrary->GetSprite(SpriteAtlusID::Player, 0);
	testBackgroundRenderer.SetSprite(shantae0);

	out_go.AddComponent<Component::Player>();

	Component::RpgMovement& movement = out_go.AddComponent<Component::RpgMovement>();
	movement.speed = 10;
	movement.currentDirection = Component::RpgMovement::None;

	Component::Collider& collider = out_go.AddComponent<Component::Collider>();
	Vector2<tFixedPoint8> halfSize = Vector2<tFixedPoint8>(2, 2) / 2;
	collider.shape = AxisAlignedBoundingBox2(halfSize * -1, halfSize);

	//Component::SpriteAnimator& animator = out_go.AddComponent<Component::SpriteAnimator>();
	//animator.SetAnimation(animationLibrary->GetSpriteAnimation(SpriteAnimationID::Idle));
}
