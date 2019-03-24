#pragma once

#include "Renderer.h"
#include "engine/math/Vector2.h"

class Sprite;

class SpriteRenderer : public Renderer
{
	Vector2<int> m_centerToCornerSizeOffset;
	Sprite* m_sprite;

public:	
	SpriteRenderer(GameObject* gameObject);
	~SpriteRenderer();

	void SetSprite(Sprite* sprite);
	void Render(Engine* engine, Camera* camera);
};
