#pragma once
#include <string>

class Engine;
class Font;

namespace Component
{
	namespace UI
	{
		struct TextRenderer
		{
			Font* m_font = nullptr;
			std::string m_str;
		};
	}
}

namespace System
{
	namespace UI
	{
		namespace TextRenderer
		{
			void Render();
		}
	}
}
