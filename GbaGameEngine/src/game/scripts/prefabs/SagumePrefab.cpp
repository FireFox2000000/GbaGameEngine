#include "SagumePrefab.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/asset/AnimationFactory.h"

#include "engine/render/SpriteRenderer.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/physics/Rigidbody.h"
#include "engine/physics/Collider.h"

void SagumePrefab::InitAnimationContainer(SpriteAtlus* sagumeSpriteAtlus, SagumeAnimationContainer* out_animationContainer)
{
	DEBUG_LOG("Loading Sagume animations");

	// TODO, once we have sprite sheet
	out_animationContainer->AddSpriteAnimation(SagumeSpriteAnimationID::Sagume_Idle, AnimationFactory::CreateSpriteAtlusSequencedAnimation(sagumeSpriteAtlus, 0, 1, 1));
}

SharedPtr<GameObject> SagumePrefab::MakeSagumePrefab(SagumeAnimationContainer& animationContainer)
{
	SharedPtr<GameObject> prefab = std::make_shared<GameObject>();
	auto defaultAnim = animationContainer.GetAsset(SagumeSpriteAnimationID::Sagume_Idle);

	Component::SpriteRenderer& spriteRenderer = prefab->AddComponent<Component::SpriteRenderer>();
	Sprite* defaultSprite = defaultAnim->keyFrames[0].sprite;
	spriteRenderer.SetSprite(defaultSprite);

	Component::SpriteAnimator& animator = prefab->AddComponent<Component::SpriteAnimator>();
	animator.SetAnimation(defaultAnim);

	Component::Rigidbody& rigidbody = prefab->AddComponent<Component::Rigidbody>();
	rigidbody.gravity = Vector2<tFixedPoint24>(0, 0);

	Component::Collider& collider = prefab->AddComponent<Component::Collider>();
	constexpr tFixedPoint8 colliderWidth = 2;
	constexpr tFixedPoint8 colliderHeight = 1;
	collider.SetAABB(
		Vector2<tFixedPoint8>((tFixedPoint8(-0.5f) * colliderWidth), tFixedPoint8(-0.5f) * colliderHeight)
		, Vector2<tFixedPoint8>((tFixedPoint8(0.5f) * colliderWidth), tFixedPoint8(0.5f) * colliderHeight)
	);

	return prefab;
}
