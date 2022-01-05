#pragma once

namespace Input
{
    class IInputMap;

	class IInputDevice
	{
    public:
        virtual bool IsConnected() const = 0;
        virtual void Update() {}
        virtual bool GetInput(const IInputMap& inputMap) const = 0;
        virtual bool GetInputDown(const IInputMap& inputMap) const = 0;
        virtual bool GetInputUp(const IInputMap& inputMap) const = 0;

        int GetInputMapId() const { return 0; }

        //DeviceType Type{ get; }
        //IInputMap GetCurrentInput(InputAction.Properties properties);
        //string GetDeviceName();

        //bool GetInputDown(IInputMap map);
        //bool GetInputUp(IInputMap map);
        //bool GetInput(IInputMap map);
        //float ? GetAxis(IInputMap map);
	};
}
