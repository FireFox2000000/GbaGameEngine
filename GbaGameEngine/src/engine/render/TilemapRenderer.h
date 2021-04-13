#pragma once
#include "Renderer.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

class Tilemap;
class GameObject;

namespace System
{
	namespace TilemapRenderer
	{
		void VBlankRender(Engine* engine, GameObject* camera);
	}
}

namespace Component
{
	class TilemapRenderer : public Renderer
	{
		friend void System::TilemapRenderer::VBlankRender(Engine* engine, GameObject* camera);

		struct MapWrappingPoints
		{
			struct WrappingPointsX
			{
				int bgTileXStart;
				int bgTileXEnd;
				int xWrappingOffsetPoint;
				int tilemapXStart;
			};

			struct WrappingPointsY
			{
				int bgTileYStart;
				int bgTileYEnd;
				int yWrappingOffsetPoint;

				int tilemapYStart;
				int tilemapYEnd;
				int tilemapYWrappingOffsetPoint;
			};

			WrappingPointsX allColumn;
			WrappingPointsY allRow;

			WrappingPointsX newColumn;
			WrappingPointsY newRow;		
		};

		/*
		* Used for rendering optimisations to track what's currently already loaded into memory. 
		* Background positions are write-only, so we need to track this manually. 
		*/
		Vector2<int> m_lastRenderPos;
		bool m_lastRenderPosValid = false;

		Tilemap* m_tilemap;
		bool m_tilemapDirty = true;		// True by default to set initial properties
		bool m_wrapping = false;
		bool m_visible = true;

		void SetDirty() { m_tilemapDirty = true; }
		MapWrappingPoints CalculateMapWrappingPoints(
			const Vector2<int>& tilemapRenderStartPos
			, const Vector2<int>& renderSize
			, const Vector2<u8>& tileMapSizeInTiles
		);

	public:
		TilemapRenderer() = default;

		void SetTilemap(Tilemap* tilemap);
		Tilemap* GetTilemap() const { return m_tilemap; }

		// Wrapping toggle only available for affine backgrounds. Wraps by default otherwise.  
		void SetWrappingEnabled(bool enabled);
		bool GetWrappingEnabled() const;

		void SetVisible(bool enabled);
		bool GetVisible() const;

		bool GetDirty() { return m_tilemapDirty; }
		void ClearDirty() { m_tilemapDirty = false; }
	};
}
