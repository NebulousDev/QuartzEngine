#include "Input.h"

namespace Quartz
{
	void Input::Update()
	{
		for (InputDeviceData& deviceData : mInputDeviceRegistry)
		{
			if (deviceData.isDirty)
			{
				if (deviceData.description.buttonCount > 0)
				{
					for (InputButtonState& buttonState : deviceData.buttonStates)
					{
						if (buttonState.buttonValue == BUTTON_STATE_DOWN && !(buttonState.lastButtonValue == BUTTON_STATE_DOWN))
						{
							// Send button down message
						}

						if (buttonState.buttonValue == BUTTON_STATE_UP && !(buttonState.lastButtonValue == BUTTON_STATE_UP))
						{
							// Send button down message
						}

						if (buttonState.buttonValue == buttonState.lastButtonValue == BUTTON_STATE_DOWN)
						{
							// Send repeat message
						}
						
						buttonState.lastButtonValue = buttonState.buttonValue;
					}

				}
			}
		}
	}

	const InputButtonState& Input::GetButtonState(InputDevice device, UInt32 buttonID) const
	{
		return mInputDeviceRegistry[device].buttonStates[buttonID];
	}

	const InputAnalogState& Input::GetAnalogState(InputDevice device, UInt32 analogID) const
	{
		return mInputDeviceRegistry[device].analogStates[analogID];
	}

	const Array<InputDeviceDescription>& Input::GetConnectedDeviceList() const
	{ 
		return mConnectedInputDevices;
	}
}

