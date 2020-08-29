#pragma once
#include "Renderer.h"
#include "engine/gba/registers/display/GBABackgroundControl.h"

class Tilemap;
class GameObject;

namespace Component
{
	class TilemapRenderer : public Renderer
	{
		Tilemap* m_tilemap;
		bool m_tilemapDirty = true;		// True by default to set initial properties
		bool m_wrapping = false;
		bool m_visible = true;

		void SetDirty() { m_tilemapDirty = true; }

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

namespace System
{
	namespace TilemapRenderer
	{
		void VBlankRender(Engine* engine, GameObject* camera);
	}
}
