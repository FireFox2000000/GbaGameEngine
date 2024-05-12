#include "GBAGraphics.h"
#include "engine/screen/Screen.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/transformation/Transform.h"
#include "GBASDK/ObjectAttributeMemory.h"

namespace GBA
{
	Graphics::DrawParams Graphics::CreateDrawParams(
		const GameObject* camera
	) const
	{
		Graphics::DrawParams drawParams;

		drawParams.cameraPosition = camera->GetComponent<Component::Transform>()->GetPosition();
		drawParams.screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);

		Vector2<int> screenSizeInTiles = Screen::GetResolution() / Gfx::Tile::PIXELS_SQRROOT_PER_TILE;
		drawParams.renderSize = screenSizeInTiles + Vector2<int>(1, 1);

		return drawParams;
	}

	void Graphics::DrawSprite(
		GBA::Gfx::Sprite* sprite
		, const Vector2<tFixedPoint8>& position
		, const Vector2<tFixedPoint24>& scale
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

			renderProperties->affineIndex = affineIndex;

			DEBUG_ASSERTMSG(scale.x != 0 && scale.y != 0, "Trying to render affine sprite of scale 0");

			// The affine matrix maps from screen space to texture space, need to tell where the pixel's colour comes from. Invert to correct for this.
			// See https://www.coranac.com/tonc/text/affine.htm for details
			Vector2<tFixedPoint8> gbaInvertedScale(1.0f / scale.x.ToFloat(), 1.0f / scale.y.ToFloat());		// Gross and slow. tFixedPoint24 overflows, tFixedPoint8 not enough precision.
			u16 gbaRotation = (rotationDegrees * DegreesToRot).ToRoundedInt();
			affineProperties->SetTransformation(gbaInvertedScale, -gbaRotation);

			// Set as double rendering to avoid clipping artifact. Also requires anchorpoint changes as this will physically double the sprite size
			renderProperties->objectMode = GBA::ObjectMode::AffineDoubleRendering;
			anchorPoint *= 2;
		}
		else
		{
			// renderProperties->SetObjectMode(GBA::Gfx::Attributes::ObjectMode::ObjNormal);	// Implicit from reconstruction in AddToRenderList, no need to call this unless we want to waste cycles
			if (scale.x < 0)
			{
				renderProperties->flipHorizontal = GBA::FlippedState::Mirrored;
			}

			if (scale.y < 0)
			{
				renderProperties->flipVertical = GBA::FlippedState::Mirrored;
			}
		}

		Vector2<tFixedPoint8> newPosition = position;
		newPosition -= drawParams.cameraPosition;											// Convert world space to relative camera space	
		newPosition.y *= -1;														// Correct for screen space starting from the top
		newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
		newPosition += drawParams.screenSpaceOffset;											// Convert to screen space
		newPosition += anchorPoint;				// Offset by sprite size to render from the center

		renderProperties->priority = DrawPriority::Layer2;
		renderProperties->screenPosX = newPosition.x.ToRoundedInt();
		renderProperties->screenPosY = newPosition.y.ToRoundedInt();
	}

	void Graphics::DrawFontSprite(GBA::Gfx::Sprite* sprite, const Vector2<tFixedPoint8>& position)
	{
		using namespace GBA::Gfx;

		ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

		renderProperties->priority = DrawPriority::Layer1;
		renderProperties->screenPosX = position.x.ToRoundedInt();
		renderProperties->screenPosY = position.y.ToRoundedInt();
	}

	void Graphics::EndFrame()
	{
		m_oamManager.DoMasterRenderIntoMemory();
	}

	void Graphics::PrepareForSceneChange()
	{
		m_oamManager.UnloadAll();
	}

	void Graphics::LoadTilemap(Tilemap& out_tilemap)
	{
		m_tilemapManager.LoadTilemap(out_tilemap);
	}

	void Graphics::Unload(Tilemap * tilemap)
	{
		m_tilemapManager.Unload(tilemap);
	}
}
