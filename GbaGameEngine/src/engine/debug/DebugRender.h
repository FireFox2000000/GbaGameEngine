#pragma once

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
	GBA::Gfx::SpriteAtlus* m_spritePrimitives;

public:
	DebugRender();
	~DebugRender();
	void RenderColliders(const GameObject* camera);
};
