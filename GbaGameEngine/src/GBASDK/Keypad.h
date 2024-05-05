#pragma once

namespace GBA
{
	enum class ButtonState : unsigned
	{
		Pressed,
		Released
	};

	struct Keypad
	{
		ButtonState A : 1,
			B : 1,
			Select : 1,
			Start : 1,
			Right : 1,
			Left : 1,
			Up : 1,
			Down : 1,
			R : 1,
			L : 1,
			: 6;	// unused
	};

	// Read-only
	const volatile Keypad* const ioRegisterKeypad = reinterpret_cast<Keypad*>(0x4000130);
}
