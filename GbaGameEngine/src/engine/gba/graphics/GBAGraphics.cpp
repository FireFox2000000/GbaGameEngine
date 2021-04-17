#include "GBAGraphics.h"
#include "engine/screen/Screen.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/transformation/Transform.h"

namespace GBA
{
	Graphics::DrawParams Graphics::CreateDrawParams(
		const GameObject* camera
	)
	{
		Graphics::DrawParams drawParams;

		drawParams.cameraPosition = camera->GetComponent<Component::Transform>()->GetPosition();
		drawParams.screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);

		Vector2<int> screenSizeInTiles = Screen::GetResolution() / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
		drawParams.renderSize = screenSizeInTiles + Vector2<int>(1, 1);

		return drawParams;
	}

	void Graphics::EndFrame()
	{
		m_oamManager.DoMasterRenderIntoMemory();
	}
}
