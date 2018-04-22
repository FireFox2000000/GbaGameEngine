#ifndef PRAGMA_ONCE_ENGINE_RENDERING_GBASPRITERENDERER_H
#define PRAGMA_ONCE_ENGINE_RENDERING_GBASPRITERENDERER_H

#include "Renderer.h"
#include "engine/math/Vector2.h"

namespace GBA
{
	class ObjectAttribute;
	class OAMManager;
	class Sprite;

	typedef volatile ObjectAttribute vObjectAttribute;

	class SpriteRenderer : public Renderer
	{
		Vector2 m_centerToCornerSizeOffset;
		GBA::vObjectAttribute* m_attributeHandle;
		GBA::OAMManager* m_oamManager;	
		GBA::Sprite* m_sprite;

	public:
		SpriteRenderer(GameObject* gameObject);
		~SpriteRenderer();

		void SetSprite(GBA::Sprite* sprite);
		void Render(Camera* camera);
	};
}

#endif
