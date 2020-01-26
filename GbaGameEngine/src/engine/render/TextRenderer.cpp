#include "TextRenderer.h"
#include "engine/engine/engine.h"
#include "engine/gba/graphics/oam/GBAOAMManager.h"
#include "engine/gameobject/ui/Text.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/screen/Screen.h"

void System::UI::TextRenderer::Render(Engine * engine)
{
	auto* entityManager = engine->GetEntityRegistry();
	GBA::OAMManager* oamManager = engine->EditComponent<GBA::OAMManager>();
	const Vector2<tFixedPoint8> screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);

	entityManager->InvokeEach<Component::UI::ScreenTransform, Component::UI::Text>(
		[&oamManager, &screenSpaceOffset]
	(Component::UI::ScreenTransform& transform, Component::UI::Text& textRenderer)
		{
			UNUSED(screenSpaceOffset);
			if (!textRenderer.m_font)
				return;

			Font& font = *textRenderer.m_font;
			const char* c_str = textRenderer.m_str.c_str();

			Vector2<tFixedPoint8> drawPosition = transform.position;
			const auto characterSize = font.GetFixedCharacterSize();

			char currentChar = *c_str;

			while (currentChar != '\0')
			{
				switch (currentChar)
				{
				case '\n':
				{
					drawPosition.x = transform.position.x;
					drawPosition.y += characterSize.y;

					break;
				}
				case ' ':
				{
					drawPosition.x += characterSize.x;
					break;
				}
				
				default:
				{
					Sprite* charSprite = font.GetSpriteForCharacter(currentChar);

					if (charSprite)
					{
						GBA::ObjectAttribute* renderProperties = oamManager->AddToRenderList(charSprite);

						Vector2<tFixedPoint8> newPosition = drawPosition;
						//newPosition *= Tile::PIXELS_SQRROOT_PER_TILE;								// Camera position units to pixel units, 8 pixels per tile/unit
						//newPosition += screenSpaceOffset;											// Convert to screen space

						renderProperties->SetPriority(GBA::DrawPriority::Layer1);
						renderProperties->SetPosition(newPosition);
						//renderProperties->SetFlippedHorizontal((int)transform.scale.x < 0);
						//renderProperties->SetFlippedVertical((int)transform.scale.y < 0);
					}
					else
					{
						// todo, show error character
					}

					drawPosition.x += characterSize.x;

					break;
				}
				}				

				currentChar = *(++c_str);
			}
		});
}
