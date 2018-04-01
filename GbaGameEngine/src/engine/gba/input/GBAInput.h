#include "../../base/Typedefs.h"
#include "../../base/Macros.h"

#pragma once

namespace GBA
{
	namespace Buttons
	{
		enum Button
		{
			A = BIT(0),
			B = BIT(1),
			Select = BIT(2),
			Start = BIT(3),	
			Right = BIT(4),	
			Left = BIT(5),	
			Up = BIT(6),	
			Down = BIT(7),	
			R = BIT(8),
			L = BIT(9),

			Count = 10
		};
	}

	class Input
	{
		static u32 m_previousInput;
		static u32 m_currentInput;

	public:
		static void Update();

		inline static bool GetKey(Buttons::Button key) { return (m_currentInput & key); }
		inline static bool GetKeyDown(Buttons::Button key) { return !(m_previousInput & key) && GetKey(key); }
		inline static bool GetKeyUp(Buttons::Button key) { return (m_previousInput & key) && !GetKey(key); }
	};
}
