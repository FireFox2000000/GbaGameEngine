#pragma once
#include "engine/base/core/stl/SharedPtr.h"
#include "engine/gameobject/GameObject.h"
#include "engine/asset/FixedAssetManager.h"

namespace SagumePrefab
{
	enum SagumeSpriteAnimationID {
		Sagume_Idle,
		Sagume_Forward,
		Sagume_Ascend,
		Sagume_Descend,

		AnimationCount
	};

	enum SagumeTilemapSetID {
		TilemapSetCount
	};

	using SagumeAnimationContainer = FixedAssetManager<
		SagumeSpriteAnimationID,
		SagumeTilemapSetID>;

	void InitAnimationContainer(SpriteAtlus* sagumeSpriteAtlus, SagumeAnimationContainer* out_animationContainer);
	SharedPtr<GameObject> MakeSagumePrefab(const SagumeAnimationContainer& animationContainer);
}
