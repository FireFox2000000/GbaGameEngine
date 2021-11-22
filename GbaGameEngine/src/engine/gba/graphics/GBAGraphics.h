#pragma once
#include "engine/gba/graphics/tiles/GBATile.h"

#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/gba/graphics/tilemap/GBATilemapManager.h"

namespace GBA
{
	namespace Gfx
	{
		class Sprite;
		class TilemapSet;
		class Tilemap;
	}
}

using Tilemap = GBA::Gfx::Tilemap;

class GameObject;

namespace Component
{
	class Camera;
}

namespace GBA
{
	class Graphics
	{
		GBA::Gfx::OAMManager m_oamManager;
		GBA::Gfx::TilemapManager m_tilemapManager;


	public:
		//namespace Tile = GBA::Gfx::Tile;

		struct DrawParams
		{
			Vector2<tFixedPoint8> cameraPosition;
			Vector2<tFixedPoint8> screenSpaceOffset;
			Vector2<int> renderSize;
		};

		DrawParams CreateDrawParams(
			const GameObject* camera
		) const;

		// Time critical function, called many times per frame, inlined for sprite renderer system speed, only called from one system anyway
		inline void DrawSprite
		(
			GBA::Gfx::Sprite* sprite
			, const Vector2<tFixedPoint8>& position
			, const Vector2 <tFixedPoint8>& scale
			, tFixedPoint8 rotationDegrees
			, Vector2<int> anchorPoint
			, bool hasAffineTransformation
			, const DrawParams& drawParams
		)
		{
			using namespace GBA::Gfx;
			ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

			if (hasAffineTransformation)
			{
				constexpr tFixedPoint8 DegreesToRot(0xFFFF / 360.f);

				u8 affineIndex = 255;
				auto* affineProperties = m_oamManager.AddToAffineRenderList(&affineIndex);
				DEBUG_ASSERTMSGFORMAT(affineIndex < 32, "Affine index out of range %d", affineIndex);

				renderProperties->SetAffineIndex(affineIndex);

				// The affine matrix maps from screen space to texture space, need to tell where the pixel's colour comes from. Invert to correct for this.
				// See https://www.coranac.com/tonc/text/affine.htm for details
				Vector2<tFixedPoint8> gbaInvertedScale(tFixedPoint8(1) / scale.x, tFixedPoint8(1) / scale.y);
				u16 gbaRotation = (rotationDegrees * DegreesToRot).ToRoundedInt();
				affineProperties->SetTransformation(gbaInvertedScale, -gbaRotation);

				// Set as double rendering to avoid clipping artifact. Also requires anchorpoint changes as this will physically double the sprite size
				renderProperties->SetObjectMode(GBA::Gfx::Attributes::ObjectMode::ObjAffineDoubleRendering);
				anchorPoint *= 2;
			}
			else
			{
				// renderProperties->SetObjectMode(GBA::Gfx::Attributes::ObjectMode::ObjNormal);	// Implicit from reconstruction in AddToRenderList, no need to call this unless we want to waste cycles
				if (scale.x < 0)
					renderProperties->SetFlippedHorizontal();

				if (scale.y < 0)
					renderProperties->SetFlippedVertical();
			}

			Vector2<tFixedPoint8> newPosition = position;
			newPosition -= drawParams.cameraPosition;											// Convert world space to relative camera space	
			newPosition.y *= -1;														// Correct for screen space starting from the top
			newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
			newPosition += drawParams.screenSpaceOffset;											// Convert to screen space
			newPosition += anchorPoint;				// Offset by sprite size to render from the center

			renderProperties->SetPriority(DrawPriority::Layer2);
			renderProperties->SetPosition(newPosition);
		}

		// Time critical function, called many times per frame, inlined for text renderer system speed, only called from one system anyway
		inline void DrawFontSprite
		(
			GBA::Gfx::Sprite* sprite
			, const Vector2<tFixedPoint8>& position
		)
		{
			using namespace GBA::Gfx;

			ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

			renderProperties->SetPriority(DrawPriority::Layer1);
			renderProperties->SetPosition(position);
		}

		void LoadTilemap(Tilemap& out_tilemap);
		void Unload(Tilemap* tilemap);

		void DrawTilemap
		(
			Tilemap* tilemap
			, const Vector2<tFixedPoint8>& position
			, const DrawParams& drawParams
		);

		void EndFrame();
		void PrepareForSceneChange();
	};
}
