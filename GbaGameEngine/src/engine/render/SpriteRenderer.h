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
		Sprite* m_sprite;

	public:
		void SetSprite(Sprite* sprite);
		Sprite* GetSprite() const { return m_sprite; }
		Vector2<int> GetCenterToCornerSizeOffset() const { return m_centerToCornerSizeOffset; }
	};
}

namespace System
{
	namespace SpriteRenderer
	{
		void Render(Engine* engine, GameObject* camera);
	}
}
