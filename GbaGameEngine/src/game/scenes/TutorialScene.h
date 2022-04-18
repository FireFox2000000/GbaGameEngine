#pragma once
#include "engine/scene/Scene.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/asset/SpriteAssetManager.h"
#include "engine/base/core/stl/SharedPtr.h"
#include "engine/gameobject/GameObject.h"
#include "engine/util/state/StateMachine.h"
#include "engine/render/UiRenderer.h"

#include "game/scripts/prefabs/SagumePrefab.h"
#include "game/scripts/GameState.h"
#include "game/scripts/util/CommandQueue.h"

struct TutorialStateParams
{
	using TutorialStateMachine = StateMachine<TutorialStateParams*>;

	UiRenderer uiRenderer;
	CommandQueue<> uiRenderCommandQueue;

	GameState gamestate;
	TutorialStateMachine stateMachine;

	// Game objects
	SharedPtr<GameObject> sagumeGameObject;
};

class TutorialScene : public Scene
{
public:

private:
	SpriteAssetManager<1, 12> m_spriteAssetManager;
	SagumePrefab::SagumeAnimationContainer m_sagumeAnimationContainer;

	SpriteAtlus* m_sagumeAtlus;
	TutorialStateParams m_sceneParams;

public:
	TutorialScene();

	void Enter() override;
	void Update() override;
	void Exit() override;
	void Render() override;
};
