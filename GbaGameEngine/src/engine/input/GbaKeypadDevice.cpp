#include "GbaKeypadDevice.h"

#include "GbaKeypadMap.h"
#include "gbatek/Keypad.h"

bool Input::GbaKeypadDevice::IsConnected() const
{
	return true;
}

void Input::GbaKeypadDevice::Update()
{
	m_inputState.Flip();
	auto& current = m_inputState.GetPrimary();
	current = 0;

	current |= (GBATEK::ioRegisterKeypad->A == GBATEK::ButtonState::Pressed) ? Buttons::A : 0;
	current |= Buttons::B * (GBATEK::ioRegisterKeypad->B == GBATEK::ButtonState::Pressed);
	current |= Buttons::Select * (GBATEK::ioRegisterKeypad->Select == GBATEK::ButtonState::Pressed);
	current |= Buttons::Start * (GBATEK::ioRegisterKeypad->Start == GBATEK::ButtonState::Pressed);
	current |= (GBATEK::ioRegisterKeypad->Right == GBATEK::ButtonState::Pressed) ? Buttons::Right : 0;
	current |= (GBATEK::ioRegisterKeypad->Left == GBATEK::ButtonState::Pressed) ? Buttons::Left : 0;
	current |= (GBATEK::ioRegisterKeypad->Up == GBATEK::ButtonState::Pressed) ? Buttons::Up : 0;
	current |= (GBATEK::ioRegisterKeypad->Down == GBATEK::ButtonState::Pressed) ? Buttons::Down : 0;
	current |= Buttons::R * (GBATEK::ioRegisterKeypad->R == GBATEK::ButtonState::Pressed);
	current |= Buttons::L * (GBATEK::ioRegisterKeypad->L == GBATEK::ButtonState::Pressed);
}

bool TestKey(Input::GbaKeypadDevice::Buttons key, const u32 inputChannel)
{
	return inputChannel & key;
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
	return GetInput((const GbaKeypadMap&)inputMap);
}

bool Input::GbaKeypadDevice::GetInputDown(const IInputMap& inputMap) const
{
	return GetInputDown((const GbaKeypadMap&)inputMap);
}

bool Input::GbaKeypadDevice::GetInputUp(const IInputMap& inputMap) const
{
	return GetInputUp((const GbaKeypadMap&)inputMap);
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
