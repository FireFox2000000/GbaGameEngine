#include "GBAGraphics.h"
#include "engine/screen/Screen.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/transformation/Transform.h"
#include "engine/gba/config/GBADrawPriorityID.h"
#include "gbatek/ObjectAttributeMemory.h"

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
		drawParams.renderSize = screenSizeInTiles + Vector2<int>{ .x = 1, .y = 1 };

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
		GBATEK::ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

		if (hasAffineTransformation)
		{
			constexpr tFixedPoint8 DegreesToRot(0xFFFF / 360.f);

			u8 affineIndex = 255;
			auto* affineProperties = m_oamManager.AddToAffineRenderList(&affineIndex);
			DEBUG_ASSERTMSGFORMAT(affineIndex < 32, "Affine index out of range %d", affineIndex);

			// Set as double rendering to avoid clipping artifact.
			renderProperties->objectMode = GBATEK::ObjectMode::AffineDoubleRendering;
			renderProperties->affineIndex = affineIndex;

			DEBUG_ASSERTMSG(scale.x != 0 && scale.y != 0, "Trying to render affine sprite of scale 0");

			// The affine matrix maps from screen space to texture space, need to tell where the pixel's colour comes from. Invert to correct for this.
			// See https://www.coranac.com/tonc/text/affine.htm for details
			// Float usage gross and slow, however tFixedPoint24 overflows, tFixedPoint8 not enough precision.
			Vector2<tFixedPoint8> gbaInvertedScale{ .x = 1.0f / scale.x.ToFloat(), .y = 1.0f / scale.y.ToFloat() };		
			u16 gbaRotation = (rotationDegrees * DegreesToRot).ToRoundedInt();

			{
				u16 rotationAlpha = -gbaRotation;
				int sinResult = Math::Sin(rotationAlpha), cosSinResult = Math::Cos(rotationAlpha);

				constexpr int shift = SIN_LUT_FRACTIONAL_BITS;

				affineProperties->paFixedPoint8 = cosSinResult * gbaInvertedScale.x.GetStorage() >> shift;
				affineProperties->pbFixedPoint8 = sinResult * gbaInvertedScale.x.GetStorage() >> shift;
				affineProperties->pcFixedPoint8 = -sinResult * gbaInvertedScale.y.GetStorage() >> shift;
				affineProperties->pdFixedPoint8 = cosSinResult * gbaInvertedScale.y.GetStorage() >> shift;
			}

			// Double rendering requires anchorpoint changes as AffineDoubleRendering will 
			// physically double the sprite size
			anchorPoint *= 2;
		}
		else
		{
			// renderProperties->SetObjectMode(GBA::Gfx::Attributes::ObjectMode::ObjNormal);	// Implicit from reconstruction in AddToRenderList, no need to call this unless we want to waste cycles
			if (scale.x < 0)
			{
				renderProperties->flipHorizontal = GBATEK::ObjectFlippedState::Mirrored;
			}

			if (scale.y < 0)
			{
				renderProperties->flipVertical = GBATEK::ObjectFlippedState::Mirrored;
			}
		}

		Vector2<tFixedPoint8> newPosition = position;
		newPosition -= drawParams.cameraPosition;											// Convert world space to relative camera space	
		newPosition.y *= -1;														// Correct for screen space starting from the top
		newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
		newPosition += drawParams.screenSpaceOffset;											// Convert to screen space
		newPosition += anchorPoint;				// Offset by sprite size to render from the center

		renderProperties->priority = static_cast<int>(GBA::DrawPriorityID::ObjSprite);
		renderProperties->screenPosX = newPosition.x.ToRoundedInt();
		renderProperties->screenPosY = newPosition.y.ToRoundedInt();
	}

	void Graphics::DrawFontSprite(GBA::Gfx::Sprite* sprite, const Vector2<tFixedPoint8>& position)
	{
		using namespace GBA::Gfx;

		GBATEK::ObjectAttribute* renderProperties = m_oamManager.AddToRenderList(sprite);

		renderProperties->priority = static_cast<int>(GBA::DrawPriorityID::ObjFontSprite);
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
