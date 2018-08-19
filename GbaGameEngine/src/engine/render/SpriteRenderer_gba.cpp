#include "SpriteRenderer.h"

#include "engine/graphicalassets/sprite/Sprite.h"
#include "engine/base/Macros.h"
#include "engine/engine/engine.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/gba/graphics/oam/GBAAttributeFunctions.h"
#include "engine/graphicalassets/tile/Tile.h"
#include "engine/gameobject/GameObject.h"
#include "engine/gameobject/Camera.h"
#include "engine/screen/Screen.h"

SpriteRenderer::SpriteRenderer(GameObject* gameObject)
	: Renderer(gameObject)
	, m_sprite(NULL)
{
}

SpriteRenderer::~SpriteRenderer()
{
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

void SpriteRenderer::Render(Engine* engine, Camera* camera)
{
	if (camera->GetProjection() != Projection::Orthographic)
		return;		// Unhandled, todo

	if (!m_sprite)
		return;

	GBA::OAMManager* oamManager = engine->GetOAMManager();
	GBA::OAMSpriteRenderProperties* renderProperties = oamManager->AddToRenderList(m_sprite);
	GBA::ObjectAttribute& oamProperties = renderProperties->oamProperties;

	Vector2f position = GetGameObject()->GetPosition2();
	position -= camera->GetPosition2();					// Convert world space to relative camera space	
	position.y *= -1;									// Correct for screen space starting from the top
	position *= Tile::PIXELS_SQRROOT_PER_TILE;			// Camera position units to pixel units, 8 pixels per tile/unit
	position += Screen::GetResolution() / 2.f;			// Convert to screen space
	position += m_centerToCornerSizeOffset;				// Offset by sprite size to render from the center

	oamProperties.SetPosition(position);
}
