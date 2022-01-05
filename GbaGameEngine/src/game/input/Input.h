#pragma once
#include "InputConfig.h"
#include "engine/input/InputManager.h"

namespace Input
{
	class IInputMap;
	class IInputDevice;

	// Handles iterating IInputMap* list when IInputMap is actually a derived type/of varying size
	struct InputMapIter
	{
		const IInputMap* mapsBegin;
		const IInputMap* mapsEnd;
		int mapSize;
	};

	bool GetInput(
		GameInputs input,
		IInputDevice* const* devicesBegin, IInputDevice* const* devicesEnd
	);

	bool GetInputDown(
		GameInputs input,
		IInputDevice* const* devicesBegin, IInputDevice* const* devicesEnd
	);

	bool GetInputUp(
		GameInputs input,
		IInputDevice* const* devicesBegin, IInputDevice* const* devicesEnd
	);

	bool GetInput(GameInputs input, const Input::InputManager::tInputDeviceList& devices);
	bool GetInputDown(GameInputs input, const Input::InputManager::tInputDeviceList& devices);
	bool GetInputUp(GameInputs input, const Input::InputManager::tInputDeviceList& devices);
}
