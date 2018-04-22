#ifndef PRAGMA_ONCE_ENGINE_GBA_REGISTERS_INPUT_GBAINPUT_H
#define PRAGMA_ONCE_ENGINE_GBA_REGISTERS_INPUT_GBAINPUT_H

#include "engine/base/Typedefs.h"
#include "engine/base/Macros.h"

namespace GBA
{
	namespace Buttons
	{
		enum Enum
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

		inline static bool GetKey(Buttons::Enum key, const u32 inputChannel) { return !(inputChannel & key); }

	public:
		static void Update();

		inline static bool GetKey(Buttons::Enum key) { return GetKey(key, m_currentInput); }
		inline static bool GetKeyDown(Buttons::Enum key) { return !GetKey(key, m_previousInput) && GetKey(key); }
		inline static bool GetKeyUp(Buttons::Enum key) { return GetKey(key, m_previousInput) && !GetKey(key); }
	};
}

#endif
