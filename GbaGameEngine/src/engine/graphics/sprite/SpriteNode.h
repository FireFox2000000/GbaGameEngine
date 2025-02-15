#pragma once
#include "engine/graphics/sprite/Sprite.h"

struct SpriteNode
{
	Sprite sprite;
	SpriteNode* next = nullptr;
};
