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
	
class Sprite;

class SpriteRenderer : public Renderer
{
	Vector2 m_centerToCornerSizeOffset;
	Sprite* m_sprite;

	GBA::vObjectAttribute* m_attributeHandle;
	GBA::OAMManager* m_oamManager;	

public:	
	SpriteRenderer(GameObject* gameObject);
	~SpriteRenderer();

	void SetSprite(Sprite* sprite);
	void Render(Camera* camera);
};

#endif
