#include "GBASpriteRenderer.h"

#include "engine\assets\GBASprite.h"
#include "engine\base\Macros.h"
#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\gba\graphics\oam\GBAAttributeFunctions.h"
#include "engine\gba\graphics\tiles\GBATileConfig.h"
#include "engine\gameobject\GameObject.h"
#include "engine\gameobject\Camera.h"
#include "engine\screen\Screen.h"

namespace GBA
{
	SpriteRenderer::SpriteRenderer(GameObject* gameObject)
		: Renderer(gameObject)
		, m_attributeHandle(NULL)
		, m_sprite(NULL)
	{
		m_oamManager = OAMManager::GetCurrent();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		if (m_attributeHandle)
			m_oamManager->Release(m_attributeHandle);
	}

	void SpriteRenderer::SetSprite(Sprite* sprite)
	{
		using namespace Attributes;

		if (sprite)
		{
			m_centerToCornerSizeOffset = AttributeFunctions::GetPixelSize(sprite->shape, sprite->sizeMode) / -2;
		}

		m_sprite = sprite;
	}

	void SpriteRenderer::Render(Camera* camera)
	{
		if (camera->GetProjection() != Projection::Orthographic)
			return;		// Unhandled, todo

		bool wantRender = m_sprite && m_sprite->isLoaded;
		if (wantRender)
		{
			if (!m_attributeHandle)
			{
				m_attributeHandle = m_oamManager->ReserveObject();

				m_attributeHandle->SetPaletteIndex(m_sprite->paletteId);
				m_attributeHandle->SetTileIndex(m_sprite->tileIndex);
				m_attributeHandle->SetShape(m_sprite->shape);
				m_attributeHandle->SetSizeMode(m_sprite->sizeMode);
			}

			Vector2 position = GetGameObject()->GetPosition2();
			position -= camera->GetPosition2();					// Convert world space to relative camera space	
			position.y *= -1;									// Correct for screen space starting from the top
			position *= TileConfig::PIXELS_SQRROOT_PER_TILE;	// Camera position units to pixel units, 8 pixels per tile/unit
			position += Screen::GetResolution() / 2.f;			// Convert to screen space
			position += m_centerToCornerSizeOffset;				// Offset by sprite size to render from the center

			m_attributeHandle->SetPosition(position);
		}
		else if (m_attributeHandle)
		{
			m_oamManager->Release(m_attributeHandle);
		}
	}
}
