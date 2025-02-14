#pragma once

class GameObject;

// Forward declaration, ideally shouldn't mention GBA
namespace GBA
{
	namespace Gfx
	{
		class Tilemap;
	}
}

using Tilemap = GBA::Gfx::Tilemap;

namespace System
{
	void RenderTilemapsVBlank(GameObject* camera);
}

class TilemapRenderer
{
	friend void RenderTilemapsVBlank(GameObject* camera);

	Tilemap* m_tilemap = nullptr;
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
