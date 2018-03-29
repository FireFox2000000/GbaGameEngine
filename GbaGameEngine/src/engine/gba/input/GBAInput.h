#include "../../Typedefs.h"

#pragma once

namespace GBA
{
	namespace Buttons
	{
		enum Button : u32
		{
			A = 1 << 0,	// 0000 0000 0000 0001, 1
			B = 1 << 1,	// 0000 0000 0000 0010, 2
			Select = 1 << 2,	// 0000 0000 0000 0100, 4
			Start = 1 << 3,	// 0000 0000 0000 1000, 8
			Right = 1 << 4,	// 0000 0000 0001 0000, 16
			Left = 1 << 5,	// 0000 0000 0010 0000, 32
			Up = 1 << 6,	// 0000 0000 0100 0000, 64
			Down = 1 << 7,	// 0000 0000 1000 0000, 128
			R = 1 << 8,	// 0000 0001 0000 0000, 256
			L = 1 << 9,	// 0000 0010 0000 0000, 512

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
