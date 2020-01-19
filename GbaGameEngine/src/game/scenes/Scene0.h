#pragma once

#include "engine/scene/Scene.h"
#include "game/scripts/rulestates/GameRulestate.h"
#include "engine/base/core/stl/Bitmask.h"

class Scene0 : public Scene
{
	GameStateMachine m_rulestateMachine;
	std::unique_ptr<GameObject> background;
	std::unique_ptr<GameObject> respawnGrid;

	void SetupSceneProps(Engine* engine);

public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;

	std::unique_ptr<GameObject> player;

	static constexpr int MAX_PROPS = 5;
	FixedList<GameObject, MAX_PROPS> propObjects;

	enum SceneFlags
	{
		Prop1Interacted,
		Prop2Interacted,
		Prop3Interacted,

		Count,
	};
	Bitmask<u32> m_sceneFlags;
};
