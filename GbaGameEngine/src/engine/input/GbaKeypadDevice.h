#pragma once

#include "IInputDevice.h"
#include "engine/base/core/stl/Bitmask.h"
#include "engine/base/core/stl/DoubleBuffer.h"

namespace Input
{
	class IInputMap;
	class GbaKeypadMap;

	class GbaKeypadDevice : public IInputDevice
	{
		DoubleBuffer<u32> m_inputState;

	public:
		enum Buttons
		{
			A = BIT(0),
			B = BIT(1),
			Select = BIT(2),
			Start = BIT(3),
			Right = BIT(4),
			Left = BIT(5),
			Up = BIT(6),
			Down = BIT(7),
			R = BIT(8),
			L = BIT(9),

			Count = 10
		};

		bool IsConnected() const override;
		void Update() override;

		bool GetKey(Buttons key) const;
		bool GetKeyDown(Buttons key) const;
		bool GetKeyUp(Buttons key) const;

		bool GetInput(const IInputMap& inputMap) const override;
		bool GetInputDown(const IInputMap& inputMap) const override;
		bool GetInputUp(const IInputMap& inputMap) const override;

		bool GetInput(const GbaKeypadMap& inputMap) const;
		bool GetInputDown(const GbaKeypadMap& inputMap) const;
		bool GetInputUp(const GbaKeypadMap& inputMap) const;
	};
}
