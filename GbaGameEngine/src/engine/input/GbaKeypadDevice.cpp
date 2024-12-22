#include "GbaKeypadDevice.h"

#include "GbaKeypadMap.h"
#include "gbatek/Keypad.h"

namespace
{
	bool TestKey(Input::GbaKeypadDevice::Buttons key, const u32 inputChannel)
	{
		return inputChannel & key;
	}
}

bool Input::GbaKeypadDevice::IsConnected() const
{
	return true;
}

void Input::GbaKeypadDevice::Update()
{
	m_inputState.Flip();
	auto& current = m_inputState.GetPrimary();
	current = 0;

	const GBATEK::Keypad keypadState = *GBATEK::ioRegisterKeypad;

	current |= (keypadState.buttons.A == GBATEK::ButtonState::Pressed) ? Buttons::A : 0;
	current |= (keypadState.buttons.B == GBATEK::ButtonState::Pressed) ? Buttons::B : 0;
	current |= (keypadState.buttons.Select == GBATEK::ButtonState::Pressed) ? Buttons::Select : 0;
	current |= (keypadState.buttons.Start == GBATEK::ButtonState::Pressed) ? Buttons::Start : 0;
	current |= (keypadState.buttons.Right == GBATEK::ButtonState::Pressed) ? Buttons::Right : 0;
	current |= (keypadState.buttons.Left == GBATEK::ButtonState::Pressed) ? Buttons::Left : 0;
	current |= (keypadState.buttons.Up == GBATEK::ButtonState::Pressed) ? Buttons::Up : 0;
	current |= (keypadState.buttons.Down == GBATEK::ButtonState::Pressed) ? Buttons::Down : 0;
	current |= (keypadState.buttons.R == GBATEK::ButtonState::Pressed) ? Buttons::R : 0;
	current |= (keypadState.buttons.L == GBATEK::ButtonState::Pressed) ? Buttons::L : 0;
}

bool Input::GbaKeypadDevice::GetKey(Buttons key) const
{
	return TestKey(key, m_inputState.GetPrimary());
}

bool Input::GbaKeypadDevice::GetKeyDown(Buttons key) const
{
	return !TestKey(key, m_inputState.GetSecondary()) && GetKey(key);
}

bool Input::GbaKeypadDevice::GetKeyUp(Buttons key) const
{
	return TestKey(key, m_inputState.GetSecondary()) && !GetKey(key);
}

bool Input::GbaKeypadDevice::GetInput(const IInputMap& inputMap) const
{
	return GetInput(static_cast<const GbaKeypadMap&>(inputMap));
}

bool Input::GbaKeypadDevice::GetInputDown(const IInputMap& inputMap) const
{
	return GetInputDown(static_cast<const GbaKeypadMap&>(inputMap));
}

bool Input::GbaKeypadDevice::GetInputUp(const IInputMap& inputMap) const
{
	return GetInputUp(static_cast<const GbaKeypadMap&>(inputMap));
}

bool InputMatches(u32 inputMask, u32 mapMask)
{
	return (inputMask & mapMask) == mapMask;
}

bool Input::GbaKeypadDevice::GetInput(const GbaKeypadMap& inputMap) const
{
	return InputMatches(m_inputState.GetPrimary(), inputMap.keysMask);
}

bool Input::GbaKeypadDevice::GetInputDown(const GbaKeypadMap& inputMap) const
{
	return !InputMatches(m_inputState.GetSecondary(), inputMap.keysMask) &&
		InputMatches(m_inputState.GetPrimary(), inputMap.keysMask);
}

bool Input::GbaKeypadDevice::GetInputUp(const GbaKeypadMap& inputMap) const
{
	return InputMatches(m_inputState.GetSecondary(), inputMap.keysMask) &&
		!InputMatches(m_inputState.GetPrimary(), inputMap.keysMask);
}
