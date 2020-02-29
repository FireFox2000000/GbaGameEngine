#pragma once
#include "engine/gba/graphics/tiles/GBATile.h"
#include "engine/gba/graphics/sprite/GBASprite.h"
#include "engine/gba/graphics/sprite/GBASpriteAtlus.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"

using Sprite = GBA::Gfx::Sprite;
using SpriteAtlus = GBA::Gfx::SpriteAtlus;

namespace GBA
{
	class Graphics
	{
		GBA::Gfx::OAMManager m_oamManager;

	public:
		//namespace Tile = GBA::Gfx::Tile;

		// Time critical function, called many times per frame, inlined for sprite renderer system speed, only called from one system anyway
		inline void DrawSprite
		(
			Sprite* sprite
			, const Vector2<tFixedPoint8>& position
			, const Vector2 <tFixedPoint8>& scale
			, const Vector2<int>& anchorPoint
			, const Vector2<tFixedPoint8>& cameraPosition
			, const Vector2<tFixedPoint8>& screenSpaceOffset
		)
		{
			using namespace GBA::Gfx;
			ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

			Vector2<tFixedPoint8> newPosition = position;
			newPosition -= cameraPosition;											// Convert world space to relative camera space	
			newPosition.y *= -1;														// Correct for screen space starting from the top
			newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			newPosition += screenSpaceOffset;											// Convert to screen space
			newPosition += anchorPoint;				// Offset by sprite size to render from the center

			renderProperties->SetPriority(DrawPriority::Layer2);

			renderProperties->SetPosition(newPosition);
			renderProperties->SetFlippedHorizontal((int)scale.x < 0);
			renderProperties->SetFlippedVertical((int)scale.y < 0);
		}

		// Time critical function, called many times per frame, inlined for text renderer system speed, only called from one system anyway
		inline void DrawFontSprite
		(
			Sprite* sprite
			, const Vector2<tFixedPoint8>& position
		)
		{
			using namespace GBA::Gfx;

			ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

			renderProperties->SetPriority(DrawPriority::Layer1);
			renderProperties->SetPosition(position);
		}

		void EndFrame();
	};
}
