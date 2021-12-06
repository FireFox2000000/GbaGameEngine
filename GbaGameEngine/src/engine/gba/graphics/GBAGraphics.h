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

		void DrawSprite
		(
			GBA::Gfx::Sprite* sprite
			, const Vector2<tFixedPoint8>& position
			, const Vector2 <tFixedPoint24>& scale
			, tFixedPoint8 rotationDegrees
			, Vector2<int> anchorPoint
			, bool hasAffineTransformation
			, const DrawParams& drawParams
		);

		void DrawFontSprite(GBA::Gfx::Sprite* sprite, const Vector2<tFixedPoint8>& position);

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
