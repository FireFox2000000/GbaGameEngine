#include "TextRenderer.h"
#include "engine/engine/engine.h"
#include "engine/transform/ui/Text.h"
#include "engine/transform/ScreenTransform.h"
#include "engine/screen/Screen.h"
#include "engine/graphics/Graphics.h"

void System::UI::TextRenderer::Render()
{
	auto* entityManager = Engine::GetInstance().GetEntityRegistry();
	Graphics* graphics = Engine::GetInstance().GetComponent<Graphics>();

	entityManager->InvokeEach<Component::UI::ScreenTransform, Component::UI::Text>(
		[&graphics]
	(Component::UI::ScreenTransform& transform, Component::UI::Text& textRenderer)
		{
			if (!textRenderer.m_font)
				return;

			Font& font = *textRenderer.m_font;
			const char* c_str = textRenderer.m_str.c_str();

			Vector2<FPi16> drawPosition = transform.position;
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
