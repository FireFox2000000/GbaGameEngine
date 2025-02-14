#pragma once

#include "engine/math/Vector2.h"

// Forward declaration, ideally shouldn't mention GBA
namespace GBA
{
	namespace Gfx
	{
		class Sprite;
	}
}

using Sprite = GBA::Gfx::Sprite;

class GameObject;

class SpriteRenderer
{
	Vector2i m_centerToCornerSizeOffset;
	Sprite* m_sprite = nullptr;

public:
	void SetSprite(Sprite* sprite);
	Sprite* GetSprite() const;
	Vector2i GetCenterToCornerSizeOffset() const;
};

namespace System
{
	// Performs screen culling across all entities that have a sprite to display and adds them to the draw list if not culled. 
	// Run this before VBlank occours. 
	void RenderSprites(GameObject* camera);
}
