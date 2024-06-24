#pragma once
#include "engine/scene/Scene.h"
#include "engine/asset/FixedAssetManager.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/audio/AudioManager.h"
#include "engine/asset/SpriteAssetManager.h"
#include "engine/asset/libraries/FontLibrary.h"

struct Collision;

class Scene0 : public Scene
{
	enum SpriteAnimationID {
		Shantae_Idle,

		AnimationCount
	};

	enum TilemapSetID {
		TilemapSetCount
	};

	SpriteAssetManager<1, 12> m_spriteAssetManager;
	FixedAssetManager<
		SpriteAnimationID,
		TilemapSetID> 
		m_assetManager;

	FixedList<GameObject, 128> m_gameObjects;
	FontLibrary m_fontLib;
	GameObject m_playerObject;
	GameObject* m_textObject;
	GameObject* m_textObjectCollision;
	AudioManager::tChannelHandle m_backgroundMusic;

	SpriteAtlus* m_shantaeAtlus;

public:
	Scene0();
	~Scene0();

	void Enter() override;
	void Update() override;
	void Exit() override;
};
