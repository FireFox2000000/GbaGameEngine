#pragma once
#include "engine/math/Vector2.h" 

#ifdef Platform_GBA
#include "engine/gba/graphics/sprite/GBASpriteGfxData.h"
#endif

class SpriteAtlas;

// Holds onto the actual sprite pixel data and defines the properties of the sprite.
class Sprite
{
private:
	SpriteAtlas* m_atlas = nullptr;
	Vector2<u8> m_tileSize;
#ifdef Platform_GBA
	// TODO should eventually be a list of sprite data to support composite sprites
	GBA::Gfx::SpriteGfxData m_gfxData;
#endif
public:
	Sprite() = default;
	~Sprite();

	const SpriteAtlas* GetAtlas() const;
	SpriteAtlas* EditAtlas();
	Vector2i GetSize() const;
	Vector2i GetSizeInPixels() const;

#ifdef Platform_GBA
	Sprite(SpriteAtlas* parentAtlas, GBA::Gfx::SpriteGfxData data)
		: m_atlas(parentAtlas)
		, m_gfxData(data)
	{}

	const GBA::Gfx::SpriteGfxData& GetGfxData() const
	{
		return m_gfxData;
	}

	GBA::Gfx::SpriteGfxData& EditGfxData()
	{ 
		return m_gfxData;
	}

	bool IsLoaded() const;
#endif
};
