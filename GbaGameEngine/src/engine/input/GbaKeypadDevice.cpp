#include "GbaKeypadDevice.h"
#include "GbaKeypadMap.h"

bool Input::GbaKeypadDevice::IsConnected() const
{
	return true;
}

void Input::GbaKeypadDevice::Update()
{
	m_inputState.Flip();
	m_inputState.GetPrimary().inputMask = GBA::Input::GetInputKeys();
}

bool Input::GbaKeypadDevice::GetKey(GBA::Buttons::Enum key) const
{
	return GBA::Input::GetKey(key, m_inputState.GetPrimary().inputMask);
}

bool Input::GbaKeypadDevice::GetKeyDown(GBA::Buttons::Enum key) const
{
	return !GBA::Input::GetKey(key, m_inputState.GetSecondary().inputMask) && GetKey(key);
}

bool Input::GbaKeypadDevice::GetKeyUp(GBA::Buttons::Enum key) const
{
	return GBA::Input::GetKey(key, m_inputState.GetSecondary().inputMask) && !GetKey(key);
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
	return (~inputMask & mapMask) == mapMask;
}

bool Input::GbaKeypadDevice::GetInput(const GbaKeypadMap& inputMap) const
{
	return InputMatches(m_inputState.GetPrimary().inputMask, inputMap.keysMask);
}

bool Input::GbaKeypadDevice::GetInputDown(const GbaKeypadMap& inputMap) const
{
	return !InputMatches(m_inputState.GetSecondary().inputMask, inputMap.keysMask) &&
		InputMatches(m_inputState.GetPrimary().inputMask, inputMap.keysMask);
}

bool Input::GbaKeypadDevice::GetInputUp(const GbaKeypadMap& inputMap) const
{
	return InputMatches(m_inputState.GetSecondary().inputMask, inputMap.keysMask) &&
		!InputMatches(m_inputState.GetPrimary().inputMask, inputMap.keysMask);
}
