#include "TextRenderer.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/font/Font.h"
#include "engine/transform/ScreenTransform.h"

void System::UI::RenderTextComponent(Graphics& gfx, const ScreenTransform& transform, const ::UI::TextRenderer& textRenderer)
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
				gfx.DrawFontSprite(charSprite, drawPosition);
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
}
