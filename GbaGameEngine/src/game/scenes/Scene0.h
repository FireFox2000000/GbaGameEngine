#ifndef PRAGMA_ONCE_GAME_SCENES_SCENE0_H
#define PRAGMA_ONCE_GAME_SCENES_SCENE0_H

#include "engine\scene\Scene.h"
#include "engine\assets\GBASprite.h"

class Scene0 : public Scene
{
	GBA::Sprite testSprite;

public:
	Scene0();
	~Scene0();
};

#endif
