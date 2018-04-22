#include "SpriteRenderer.h"

#include "engine\assets\Sprite.h"
#include "engine\base\Macros.h"
#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\gameobject\GameObject.h"
#include "engine\gba\graphics\oam\GBAAttributeFunctions.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: Renderer(gameObject)
	, m_attributeHandle(NULL)
	, m_sprite(NULL)
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
		m_centerToCornerSizeOffset = GBA::AttributeFunctions::GetPixelSize(sprite->shape, sprite->sizeMode) / -2;
	}

	m_sprite = sprite;
}

void SpriteRenderer::Render()
{
	using namespace GBA;

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

		Vector2 position = m_gameObject->GetPosition2();
		// Convert world space to camera space
		// Camera space to screen space
		position.y *= -1;		// Screen space starts from the top left

		position += m_centerToCornerSizeOffset;			// Offset by sprite size to render from the center

		m_attributeHandle->SetPosition(position);
	}
	else if (m_attributeHandle)
	{
		m_oamManager->Release(m_attributeHandle);
	}
}
