#ifndef PRAGMA_ONCE_ENGINE_GRAPHICALASSETS_SPRITE_SPRITELIBRARY_H
#define PRAGMA_ONCE_ENGINE_GRAPHICALASSETS_SPRITE_SPRITELIBRARY_H

#include "SpriteAtlus.h"
#include "engine/base/core/stl/List.h"

// Remember to #include the sprite data in GBASpriteManager.cpp
#define SPRITE_ATLUS_LIST \
	SPRITE_ATLUS_ENTRY(Shantae_Idle)

namespace SpriteAtlusID
{
#define SPRITE_ATLUS_ENTRY(Namespace) Namespace,
	enum Enum
	{
		SPRITE_ATLUS_LIST
		Count
	};
#undef SPRITE_ATLUS_ENTRY
}

class SpriteLibrary
{
	FixedList<SpriteAtlus, SpriteAtlusID::Count> m_spriteAtlusCollection;

public:
	SpriteLibrary();
	~SpriteLibrary();

	Sprite* GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex);
};

#endif
