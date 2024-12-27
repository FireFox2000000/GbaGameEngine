#pragma once

class Engine;
class GameObject;

namespace GBA
{
	namespace Gfx
	{
		class SpriteAtlas;
		class Sprite;
	}
}

class DebugRender
{
	GBA::Gfx::SpriteAtlas* m_spritePrimitives;

public:
	DebugRender();
	~DebugRender();
	void RenderColliders(const GameObject* camera);
};
