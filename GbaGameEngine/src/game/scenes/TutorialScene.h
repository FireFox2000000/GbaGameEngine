#pragma once
#include "engine/scene/Scene.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/asset/SpriteAssetManager.h"
#include "engine/base/core/stl/SharedPtr.h"
#include "engine/gameobject/GameObject.h"
#include "game/scripts/prefabs/SagumePrefab.h"

class TutorialScene : public Scene
{
	SpriteAssetManager<1, 12> m_spriteAssetManager;
	SagumePrefab::SagumeAnimationContainer m_sagumeAnimationContainer;

	SpriteAtlus* m_sagumeAtlus;
	SharedPtr<GameObject> m_sagumeGameObject;

public:
	TutorialScene();

	void Enter() override;
	void Update() override;
	void Exit() override;
};
