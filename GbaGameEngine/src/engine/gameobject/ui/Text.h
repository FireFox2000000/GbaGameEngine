#pragma once
#include <string>
#include "engine/graphics/font/Font.h"

class Font;

namespace Component
{
	namespace UI
	{
		struct Text
		{
			Font* m_font = nullptr;
			std::string m_str;
		};
	}
}