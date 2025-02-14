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
			Vector2<FPi16> cameraPosition;
			Vector2<FPi16> screenSpaceOffset;
			Vector2i renderSize;
		};

		DrawParams CreateDrawParams(
			const GameObject* camera
		) const;

		void DrawSprite
		(
			GBA::Gfx::Sprite* sprite
			, const Vector2<FPi16>& position
			, const Vector2 <FPi16>& scale
			, FPi16 rotationDegrees
			, Vector2i anchorPoint
			, const DrawParams& drawParams
		);

		void DrawFontSprite(GBA::Gfx::Sprite* sprite, const Vector2<FPi16>& position);

		void LoadTilemap(Tilemap& out_tilemap);
		void Unload(Tilemap* tilemap);

		void DrawTilemap
		(
			Tilemap* tilemap
			, const Vector2<FPi16>& position
			, const DrawParams& drawParams
		);

		void EndFrame();
		void PrepareForSceneChange();
	};
}
