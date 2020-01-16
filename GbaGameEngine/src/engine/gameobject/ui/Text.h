#pragma once
#include <string>
#include "engine/graphicalassets/font/Font.h"

class Font;

namespace Component
{
	namespace UI
	{
		struct Text
		{
			Font* m_font = NULL;
			std::string m_str;
		};
	}
}