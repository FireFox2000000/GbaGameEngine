#pragma once

#include "IInputDevice.h"
#include "engine/gba/registers/input/GBAInput.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/base/core/stl/DoubleBuffer.h"

namespace Input
{
	class IInputMap;
	class GbaKeypadMap;

	class GbaKeypadDevice : public IInputDevice
	{
		struct InputState
		{
			u32 inputMask;
		};

		DoubleBuffer<InputState> m_inputState;

	public:
		bool IsConnected() const override;
		void Update() override;

		bool GetKey(GBA::Buttons::Enum key) const;
		bool GetKeyDown(GBA::Buttons::Enum key) const;
		bool GetKeyUp(GBA::Buttons::Enum key) const;

		bool GetInput(const IInputMap& inputMap) const override;
		bool GetInputDown(const IInputMap& inputMap) const override;
		bool GetInputUp(const IInputMap& inputMap) const override;

		bool GetInput(const GbaKeypadMap& inputMap) const;
		bool GetInputDown(const GbaKeypadMap& inputMap) const;
		bool GetInputUp(const GbaKeypadMap& inputMap) const;
	};
}
