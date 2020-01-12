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
		GBA::BackgroundControl::Backgrounds m_backgroundSlotId = GBA::BackgroundControl::Count;
		bool m_tilemapDirty = false;

	public:
		TilemapRenderer() = default;
		~TilemapRenderer();

		void SetTilemap(Tilemap* tilemap);
		Tilemap* GetTilemap() const { return m_tilemap; }

		// Functions for System::TilemapRenderer
		bool AssignBackgroundSlot();
		void FreeAssignedBackgroundSlot();
		inline bool IsBackgroundSlotAssigned() const { return m_backgroundSlotId != GBA::BackgroundControl::Count; }
		inline GBA::BackgroundControl::Backgrounds GetAssignedBackgroundSlot() const { return m_backgroundSlotId; }
		void SetDirty() { m_tilemapDirty = true; }
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
