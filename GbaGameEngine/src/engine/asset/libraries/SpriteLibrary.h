#pragma once

#include "engine/graphicalassets/sprite/SpriteAtlus.h"
#include "engine/base/core/stl/List.h"

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

	void AddSpriteSheet(
		const u32 spriteCount, 
		const u8 paletteLength,
		const u16* palette,
		const u8* widthMap,
		const u8* heightMap,
		const u32 dataLength,
		const u32* data,
		const u32* offsets);

public:
	SpriteLibrary();
	~SpriteLibrary();

	Sprite* GetSprite(SpriteAtlusID::Enum atlusId, u32 spriteIndex);
};
