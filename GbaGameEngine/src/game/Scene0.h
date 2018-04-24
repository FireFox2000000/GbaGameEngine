#ifndef PRAGMA_ONCE_GAME_SCENE0_H
#define PRAGMA_ONCE_GAME_SCENE0_H

#include "engine\scene\Scene.h"
#include "engine\gameobject\GameObject.h"
#include "engine\gameobject\Camera.h"
#include "engine\rendering\GBASpriteRenderer.h"
#include "engine\base\core\stl\List.h"
#include "engine\assets\GBASprite.h"

class Scene0 : public Scene
{
	Camera mainCamera;
	List<GameObject> gameObjects;
	List<GBA::SpriteRenderer> renderList;

	GBA::Sprite testSprite;

public:
	Scene0();
	~Scene0();

	void Update();
	void Render();
};

#endif
