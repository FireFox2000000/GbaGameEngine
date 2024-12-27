#pragma once
#include "engine/scene/Scene.h"
#include "engine/animation/SpriteAnimator.h"
#include "engine/audio/AudioManager.h"
#include "engine/asset/libraries/FontLibrary.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlas.h"

struct Collision;

class Scene0 : public Scene
{
	FixedList<GameObject, 128> m_gameObjects;
	FontLibrary m_fontLib;
	GameObject m_playerObject;
	GameObject* m_textObject = nullptr;
	GameObject* m_textObjectCollision = nullptr;
	AudioManager::tChannelHandle m_backgroundMusic = 0;

	// Asset resources
	GBA::Gfx::SpriteAtlas* m_shantaeAtlas = nullptr;
	SpriteAnimation* m_shantaeIdleAnim = nullptr;

public:
	Scene0();
	~Scene0();

	void Enter() override;
	void Update() override;
	void Exit() override;
};
