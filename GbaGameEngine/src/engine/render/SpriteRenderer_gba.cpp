#include "SpriteRenderer.h"

#include "engine\graphicalassets\sprite\Sprite.h"
#include "engine\base\Macros.h"
#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\gba\graphics\oam\GBAAttributeFunctions.h"
#include "engine\graphicalassets\tile\Tile.h"
#include "engine\gameobject\GameObject.h"
#include "engine\gameobject\Camera.h"
#include "engine\screen\Screen.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: Renderer(gameObject)
	, m_sprite(NULL)
	, m_attributeHandle(NULL)
{
	m_oamManager = GBA::OAMManager::GetCurrent();
}

SpriteRenderer::~SpriteRenderer()
{
	if (m_attributeHandle)
		m_oamManager->Release(m_attributeHandle);
}

void SpriteRenderer::SetSprite(Sprite* sprite)
{
	using namespace GBA::Attributes;

	if (sprite)
	{
		m_centerToCornerSizeOffset = GBA::AttributeFunctions::GetPixelSize(sprite->GetShape(), sprite->GetSizeMode()) / -2;
	}

	m_sprite = sprite;
}

void SpriteRenderer::Render(Camera* camera)
{
	if (camera->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	bool wantRender = m_sprite && m_sprite->IsLoaded();
	if (wantRender)
	{
		if (!m_attributeHandle)
		{
			m_attributeHandle = m_oamManager->ReserveObject();

			m_attributeHandle->SetPaletteIndex(m_sprite->GetPaletteIndex());
			m_attributeHandle->SetTileIndex(m_sprite->GetTileIndex());
			m_attributeHandle->SetShape(m_sprite->GetShape());
			m_attributeHandle->SetSizeMode(m_sprite->GetSizeMode());
		}

		Vector2 position = GetGameObject()->GetPosition2();
		position -= camera->GetPosition2();					// Convert world space to relative camera space	
		position.y *= -1;									// Correct for screen space starting from the top
		position *= Tile::PIXELS_SQRROOT_PER_TILE;	// Camera position units to pixel units, 8 pixels per tile/unit
		position += Screen::GetResolution() / 2.f;			// Convert to screen space
		position += m_centerToCornerSizeOffset;				// Offset by sprite size to render from the center

		m_attributeHandle->SetPosition(position);
	}
	else if (m_attributeHandle)
	{
		m_oamManager->Release(m_attributeHandle);
	}
}
