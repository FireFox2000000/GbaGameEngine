#ifndef PRAGMA_ONCE_GAME_SCENES_SCENE0_H
#define PRAGMA_ONCE_GAME_SCENES_SCENE0_H

#include "engine\scene\Scene.h"
#include "engine\graphicalassets\sprite\SpriteLibrary.h"

class Scene0 : public Scene
{
	SpriteLibrary m_spriteLib;

public:
	Scene0();
	~Scene0();
};

#endif
