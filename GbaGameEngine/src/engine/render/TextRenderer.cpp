#include "TextRenderer.h"
#include "engine/engine/engine.h"
#include "engine/gameobject/ui/Text.h"
#include "engine/gameobject/ui/ScreenTransform.h"
#include "engine/screen/Screen.h"
#include "engine/graphicalassets/Graphics.h"

void System::UI::TextRenderer::Render(Engine * engine)
{
	auto* entityManager = engine->GetEntityRegistry();
	Graphics* graphics = engine->EditComponent<Graphics>();
	const Vector2<tFixedPoint8> screenSpaceOffset = Screen::GetResolution() / tFixedPoint8(2);

	entityManager->InvokeEach<Component::UI::ScreenTransform, Component::UI::Text>(
		[&graphics, &screenSpaceOffset]
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
					auto* charSprite = font.GetSpriteForCharacter(currentChar);

					if (charSprite)
					{
						graphics->DrawFontSprite(charSprite, drawPosition);
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
