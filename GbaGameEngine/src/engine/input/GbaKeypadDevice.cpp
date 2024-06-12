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

	current |= (GBA::ioRegisterKeypad->A == GBA::ButtonState::Pressed) ? Buttons::A : 0;
	current |= Buttons::B * (GBA::ioRegisterKeypad->B == GBA::ButtonState::Pressed);
	current |= Buttons::Select * (GBA::ioRegisterKeypad->Select == GBA::ButtonState::Pressed);
	current |= Buttons::Start * (GBA::ioRegisterKeypad->Start == GBA::ButtonState::Pressed);
	current |= (GBA::ioRegisterKeypad->Right == GBA::ButtonState::Pressed) ? Buttons::Right : 0;
	current |= (GBA::ioRegisterKeypad->Left == GBA::ButtonState::Pressed) ? Buttons::Left : 0;
	current |= (GBA::ioRegisterKeypad->Up == GBA::ButtonState::Pressed) ? Buttons::Up : 0;
	current |= (GBA::ioRegisterKeypad->Down == GBA::ButtonState::Pressed) ? Buttons::Down : 0;
	current |= Buttons::R * (GBA::ioRegisterKeypad->R == GBA::ButtonState::Pressed);
	current |= Buttons::L * (GBA::ioRegisterKeypad->L == GBA::ButtonState::Pressed);
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
