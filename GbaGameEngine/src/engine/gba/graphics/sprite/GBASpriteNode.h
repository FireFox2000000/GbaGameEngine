#pragma once
#include "GBASprite.h"

namespace GBA
{
	namespace Gfx
	{
		struct SpriteNode
		{
			Sprite sprite;
			SpriteNode* next = nullptr;
			// SpriteNode* nextSibling = nullptr;	// TODO, implement me for composite sprites
		};
	}
}
