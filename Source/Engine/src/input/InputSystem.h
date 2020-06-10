#pragma once

#include "../system/System.h"
#include "PlatformInput.h"
#include "util\RefPtr.h"

namespace Quartz
{
	class InputAction
	{
	protected:
		InputDeviceId	deviceId;
		Float32			value;
	};

	struct KeyboardInputAction : public InputAction
	{
		UInt32 scancode;
	};

	struct MouseButtonInputAction : public InputAction
	{
		UInt32 buttonIndex;
	};

	struct MouseWheelInputAction : public InputAction
	{
	};

	struct MouseMoveXInputAction : public InputAction
	{
		UInt32 mouseX;
	};

	struct MouseMoveYInputAction : public InputAction
	{
		UInt32 mouseY;
	};

	struct AnalogXInputAction : public InputAction
	{
		UInt32 analogX;
	};

	class InputMapping
	{
		String						inputName;
		Array<RefPtr<InputAction>>	inputAction;
	};

	class QUARTZ_API InputSystem : public System
	{
	private:
		friend void MouseMoveInputCallback(InputDeviceId deviceId, Int64 relX, Int64 relY);
		friend void MouseButtonInputCallback(InputDeviceId deviceId, UInt32 button, ButtonState state);
		friend void MouseWheelInputCallback(InputDeviceId deviceId, Float32 value);

	public:
		//void MapKeyboardInput(String localName, InputDeviceId device, UInt32 scancode);

		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}