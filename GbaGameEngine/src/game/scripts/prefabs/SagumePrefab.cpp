#include "SagumePrefab.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/asset/AnimationFactory.h"

void SagumePrefab::InitAnimationContainer(SpriteAtlus* sagumeSpriteAtlus, SagumeAnimationContainer* out_animationContainer)
{
	// TODO, once we have sprite sheet
	//out_animationContainer->AddSpriteAnimation(SagumeSpriteAnimationID::Sagume_Idle, AnimationFactory::CreateSpriteAtlusSequencedAnimation(sagumeSpriteAtlus, 0, 1, 1));

}

SharedPtr<GameObject> SagumePrefab::MakeSagumePrefab(const SagumeAnimationContainer& animationContainer)
{
	SharedPtr<GameObject> prefab = std::make_shared<GameObject>();

	return prefab;
}
