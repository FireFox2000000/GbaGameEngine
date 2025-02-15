#pragma once

class Engine;
class GameObject;
class SpriteAtlas;
class Sprite;

class DebugRender
{
	SpriteAtlas* m_spritePrimitives;

public:
	DebugRender();
	~DebugRender();
	void RenderColliders(const GameObject* camera);
};
