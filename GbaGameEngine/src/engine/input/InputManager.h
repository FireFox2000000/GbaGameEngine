#pragma once
#include "engine/input/IInputDevice.h"
#include "engine/input/GbaKeypadDevice.h"
#include "engine/base/core/stl/List.h"

namespace Input
{
	class InputManager
	{
	public:
		using tInputDeviceList = FixedList<Input::IInputDevice*, 1>;

	private:
		tInputDeviceList m_inputDevices = { new GbaKeypadDevice() };

	public:
		void Update();
		const tInputDeviceList& GetDevices() { return m_inputDevices; }
	};
}