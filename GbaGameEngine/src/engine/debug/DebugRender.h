#pragma once
#include "engine/asset/SpriteAssetManager.h"

class Engine;
class GameObject;

namespace GBA
{
	namespace Gfx
	{
		class SpriteAtlus;
		class Sprite;
	}
}

class DebugRender
{
	SpriteAssetManager<1, 2> m_spriteAssetManager;
	GBA::Gfx::SpriteAtlus* spritePrimitives;

public:
	DebugRender();
	void RenderColliders(const GameObject* camera);
};
