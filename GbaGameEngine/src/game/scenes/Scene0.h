#pragma once
#include "engine/scene/Scene.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/audio/AudioManager.h"

class Scene0 : public Scene
{
	enum SpriteAtlusID {
		Shantae,

		SpriteCount
	};

	enum SpriteAnimationID {
		Shantae_Idle,

		AnimationCount
	};

	enum TilemapSetID {
		TilemapSetCount
	};

	FixedAssetManager<
		SpriteAtlusID, 
		SpriteAnimationID,
		TilemapSetID> 
		m_assetManager;

	FixedList<GameObject, 128> m_gameObjects;

	GameObject playerObject;
	GameObject* textObject;
	GameObject* textObjectCollision;
	AudioManager::tChannelHandle m_backgroundMusic;

public:
	Scene0(Engine* engine);
	~Scene0();

	void Enter(Engine* engine) override;
	void Update(Engine* engine) override;
	void Exit(Engine* engine) override;
};
