#include "SpriteRenderer.h"

#include "engine\base\Macros.h"
#include "engine\gba\graphics\oam\GBAOAMManager.h"
#include "engine\gameobject\GameObject.h"


SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: Renderer(gameObject)
	, m_attributeHandle(NULL)
{
	m_oamManager = GBA::OAMManager::GetCurrent();
}

SpriteRenderer::~SpriteRenderer()
{
	if (m_attributeHandle)
		m_oamManager->Release(m_attributeHandle);
}

void SpriteRenderer::SetSprite()
{
	// TODO: Calculate sprite offset
	m_centerToCornerSizeOffset = Vector2::Zero;
}

void SpriteRenderer::Render()
{
	using namespace GBA;

	bool needToRender = true;	// True
	if (needToRender)
	{
		if (!m_attributeHandle)
		{
			m_attributeHandle = m_oamManager->ReserveObject();

			// TODO: Initialise based on actual sprite data
			m_attributeHandle->SetPaletteIndex(0);
			m_attributeHandle->SetBaseTileIndex(4);
			m_attributeHandle->SetSizeMode(Attributes::Form1);
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
