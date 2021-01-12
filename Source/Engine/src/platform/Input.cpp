#include "Input.h"

namespace Quartz
{
	void PlatformInput::SetDeviceConnectCallback(InputDeviceConnectCallbackFunc deviceConnectCallback)
	{
		mDeviceConnectCallbackFunc = deviceConnectCallback;
	}

	void PlatformInput::SetDeviceDisconnectCallback(InputDeviceDisconnectCallbackFunc deviceDisconnectCallback)
	{
		mDeviceDisconnectCallbackFunc = deviceDisconnectCallback;
	}

	void PlatformInput::SetMouseMoveInputCallback(MouseMoveInputCallbackFunc mouseMoveInputCallback)
	{
		mMouseMoveInputCallbackFunc = mouseMoveInputCallback;
	}

	void PlatformInput::SetMouseButtonInputCallback(MouseButtonInputCallbackFunc mouseButtonInputCallback)
	{
		mMouseButtonInputCallbackFunc = mouseButtonInputCallback;
	}

	void PlatformInput::SetMouseWheelInputCallback(MouseWheelInputCallbackFunc mouseWheelInputCallback)
	{
		mMouseWheelInputCallbackFunc = mouseWheelInputCallback;
	}

	void PlatformInput::SetKeyboardInputCallback(KeyboardInputCallbackFunc keyboardInputCallback)
	{
		mKeyboardInputCallbackFunc = keyboardInputCallback;
	}

	void PlatformInput::SetButtonInputCallback(ButtonInputCallbackFunc buttonInputCallback)
	{
		mButtonInputCallbackFunc = buttonInputCallback;
	}

	void PlatformInput::SetAnalogInputCallback(AnalogInputCallbackFunc analogInputCallback)
	{
		mAnalogInputCallbackFunc = analogInputCallback;
	}

	Bool8 PlatformInput::IsDeviceConnected(InputDeviceId deviceId)
	{
		return mConnectedDevices.Contains(deviceId);
	}

	InputDeviceInfo* PlatformInput::GetDeviceInfo(InputDeviceId deviceId)
	{
		return mDeviceInfos.Get(deviceId);
	}
}

