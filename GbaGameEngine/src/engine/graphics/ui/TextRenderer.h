#pragma once
#include <string>

class Graphics;
class Font;
class ScreenTransform;

namespace UI
{
	struct TextRenderer
	{
		Font* m_font = nullptr;
		std::string m_str;
	};
}

namespace System::UI
{
	void RenderTextComponent(Graphics& gfx, const ScreenTransform& transform, const ::UI::TextRenderer& textRenderer);
}
