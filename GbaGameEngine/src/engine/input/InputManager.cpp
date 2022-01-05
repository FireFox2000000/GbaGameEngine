#include "InputManager.h"

void Input::InputManager::Update()
{
	for (auto& inputDevice : m_inputDevices)
	{
		inputDevice->Update();
	}
}
