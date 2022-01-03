#pragma once
#include "engine/input/IInputDevice.h"
#include "engine/input/GbaKeypadDevice.h"
#include "engine/base/core/stl/List.h"

namespace Input
{
	class InputManager
	{
		FixedList<Input::IInputDevice, 1> m_inputDevices = { GbaKeypadDevice() };

	public:
		void Update();
	};
}