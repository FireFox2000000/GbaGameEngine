#pragma once

namespace GBATEK
{
	enum class ButtonState : unsigned short
	{
		// Pressed as 0 is intentional, an active bit in the register means that the key is not being pressed
		Pressed,
		Released
	};

	union Keypad
	{
		// TODO replace union with bitcast?
	private:
		unsigned short data;

	public:
		struct {
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
		} buttons;

		Keypad(const volatile Keypad& other)
			: data(other.data)
		{}
	};

	// Read-only
	const volatile Keypad* const ioRegisterKeypad = reinterpret_cast<const volatile Keypad* const>(0x4000130);

	static_assert(sizeof(*ioRegisterKeypad) == 2, "Keypad struct malformed");
}
