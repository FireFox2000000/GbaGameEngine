#include "Input.h"
#include "engine/Input/IInputDevice.h"
#include "engine/Input/IInputMap.h"
#include "engine/Input/GbaKeypadMap.h"

bool Input::GetInput(GameInputs input, IInputDevice* const* devicesBegin, IInputDevice* const* devicesEnd)
{
	for (auto device = devicesBegin; device != devicesEnd; ++device)
	{
		const IInputDevice* d = (*device);
		const auto inputMapIter = InputConfig::GetGameInputIter(input, d->GetInputMapId());

		for (const IInputMap* map = inputMapIter.mapsBegin; map != inputMapIter.mapsEnd; map = (const IInputMap*)((u8*)map + inputMapIter.mapSize))
		{
			if (d->GetInput(*map))
			{
				return true;
			}
		}
	}

	return false;
}

bool Input::GetInputDown(GameInputs input, IInputDevice* const* devicesBegin, IInputDevice* const* devicesEnd)
{
	for (auto device = devicesBegin; device != devicesEnd; ++device)
	{
		const IInputDevice* d = (*device);
		const auto inputMapIter = InputConfig::GetGameInputIter(input, d->GetInputMapId());

		for (const IInputMap* map = inputMapIter.mapsBegin; map != inputMapIter.mapsEnd; map = (const IInputMap*)((u8*)map + inputMapIter.mapSize))
		{
			if (d->GetInputDown(*map))
			{
				return true;
			}
		}
	}

	return false;
}

bool Input::GetInputUp(GameInputs input, IInputDevice* const* devicesBegin, IInputDevice* const* devicesEnd)
{
	for (auto device = devicesBegin; device != devicesEnd; ++device)
	{
		const IInputDevice* d = (*device);
		const auto inputMapIter = InputConfig::GetGameInputIter(input, d->GetInputMapId());

		for (const IInputMap* map = inputMapIter.mapsBegin; map != inputMapIter.mapsEnd; map = (const IInputMap*)((u8*)map + inputMapIter.mapSize))
		{
			if (d->GetInputUp(*map))
			{
				return true;
			}
		}
	}

	return false;
}

bool Input::GetInput(GameInputs input, const Input::InputManager::tInputDeviceList& devices)
{
	return Input::GetInput(input, devices.begin(), devices.end());
}

bool Input::GetInputDown(GameInputs input, const Input::InputManager::tInputDeviceList& devices)
{
	return Input::GetInputDown(input, devices.begin(), devices.end());
}

bool Input::GetInputUp(GameInputs input, const Input::InputManager::tInputDeviceList& devices)
{
	return Input::GetInputUp(input, devices.begin(), devices.end());
}
