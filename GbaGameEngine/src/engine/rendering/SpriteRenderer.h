#ifndef PRAGMA_ONCE_ENGINE_RENDERING_SPRITERENDERER_H
#define PRAGMA_ONCE_ENGINE_RENDERING_SPRITERENDERER_H

#include "Renderer.h"
#include "engine/math/Vector2.h"

namespace GBA
{
	class ObjectAttribute;
	class OAMManager;

	typedef volatile ObjectAttribute vObjectAttribute;
}

class GameObject;

class SpriteRenderer : public Renderer
{
	GBA::vObjectAttribute* m_attributeHandle;
	GBA::OAMManager* m_oamManager;
	Vector2 m_centerToCornerSizeOffset;

public:
	SpriteRenderer(GameObject* gameObject);
	~SpriteRenderer();

	void SetSprite();
	void Render();
};

#endif
