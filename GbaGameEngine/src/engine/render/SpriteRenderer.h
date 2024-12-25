#pragma once

#include "Renderer.h"
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

namespace Component
{
	class SpriteRenderer : public Renderer
	{
		Vector2<int> m_centerToCornerSizeOffset;
		Sprite* m_sprite = nullptr;

	public:
		void SetSprite(Sprite* sprite);
		Sprite* GetSprite() const;
		Vector2<int> GetCenterToCornerSizeOffset() const;
	};
}

namespace System
{
	namespace SpriteRenderer
	{
		// Performs screen culling across all entities that have a sprite to display and adds them to the draw list if not culled. 
		// Run this before VBlank occours. 
		void Render(GameObject* camera);
	}
}
