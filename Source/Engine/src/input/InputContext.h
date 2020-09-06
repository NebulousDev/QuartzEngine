#pragma once

#include "Common.h"
#include "util/String.h"
#include "util/Map.h"

#include "InputState.h"

#define ANY_DEVICE 0
#define ANY_BUTTON 0
#define ANY_ANALOG 0

#define MOUSE_X 0
#define MOUSE_Y 1

namespace Quartz
{
	typedef UInt64 InputId;
	typedef UInt64 InputDeviceId;

	class QUARTZ_API InputContext
	{
	public:
		friend class InputHandler;
		friend class InputState;

		struct KeyboardInputEntry
		{
			InputDeviceId	device;
			UInt32			scancode;

			FORCE_INLINE Bool8 operator==(const KeyboardInputEntry& entry)
			{
				return device == entry.device && scancode == entry.scancode;
			}
		};

		struct ButtonInputEntry
		{
			InputDeviceId	device;
			UInt32			button;

			FORCE_INLINE Bool8 operator==(const ButtonInputEntry& entry)
			{
				return device == entry.device && button == entry.button;
			}
		};

		struct AnalogInputEntry
		{
			InputDeviceId	device;
			UInt32			analog;

			FORCE_INLINE Bool8 operator==(const AnalogInputEntry& entry)
			{
				return device == entry.device && analog == entry.analog;
			}
		};

		struct MouseMoveInputEntry
		{
			InputDeviceId	device;
			UInt32			axis;

			FORCE_INLINE Bool8 operator==(const MouseMoveInputEntry& entry)
			{
				return device == entry.device && axis == entry.axis;
			}
		};

		struct MouseButtonInputEntry
		{
			InputDeviceId	device;
			UInt32			button;

			FORCE_INLINE Bool8 operator==(const MouseButtonInputEntry& entry)
			{
				return device == entry.device && button == entry.button;
			}
		};

		struct MouseWheelInputEntry
		{
			InputDeviceId	device;

			FORCE_INLINE Bool8 operator==(const MouseWheelInputEntry& entry)
			{
				return device == entry.device;
			}
		};

	private:
		String mName;

		Map<KeyboardInputEntry, InputBinding>		mKeyboardInputs;
		Map<ButtonInputEntry, InputBinding>			mButtonInputs;
		Map<AnalogInputEntry, InputBinding>			mAnalogInputs;
		Map<MouseMoveInputEntry, InputBinding>		mMouseMoveInputs;
		Map<MouseButtonInputEntry, InputBinding>	mMouseButtonInputs;
		Map<MouseWheelInputEntry, InputBinding>		mMouseWheelInputs;

	public:
		InputContext(String name);

	public:
		void BindKeyboardInput(String inputName, InputDeviceId device, UInt32 scancode, Float32 scale);
		void BindControllerButtonInput(String inputName, InputDeviceId device, UInt32 button, Float32 scale);
		void BindControllerAnalogInput(String inputName, InputDeviceId device, UInt32 analog, Float32 scale);
		void BindMouseMoveInput(String inputName, InputDeviceId device, UInt32 axis, Float32 scale);
		void BindMouseButtonInput(String inputName, InputDeviceId device, UInt32 button, Float32 scale);
		void BindMouseWheelInput(String inputName, InputDeviceId device, Float32 scale);
	
		InputBinding GetKeyboardBinding(InputDeviceId device, UInt32 scancode);
		InputBinding GetControllerButtonBinding(InputDeviceId device, UInt32 button);
		InputBinding GetControllerAnalogBinding(InputDeviceId device, UInt32 analog);
		InputBinding GetMouseMoveBinding(InputDeviceId device, UInt32 axis);
		InputBinding GetMouseButtonBinding(InputDeviceId device, UInt32 button);
		InputBinding GetMouseWheelBinding(InputDeviceId device);

		Bool8 HasKeyboardBinding(InputDeviceId device, UInt32 scancode);
		Bool8 HasControllerButtonBinding(InputDeviceId device, UInt32 button);
		Bool8 HasControllerAnalogBinding(InputDeviceId device, UInt32 analog);
		Bool8 HasMouseMoveBinding(InputDeviceId device, UInt32 axis);
		Bool8 HasMouseButtonBinding(InputDeviceId device, UInt32 button);
		Bool8 HasMouseWheelBinding(InputDeviceId device);
	};

	template<>
	FORCE_INLINE UInt32 Hash<InputContext::KeyboardInputEntry>(const InputContext::KeyboardInputEntry& value)
	{
		return value.device * (value.scancode + 1);
	}

	template<>
	FORCE_INLINE UInt32 Hash<InputContext::ButtonInputEntry>(const InputContext::ButtonInputEntry& value)
	{
		return value.device * (value.button + 1);
	}

	template<>
	FORCE_INLINE UInt32 Hash<InputContext::AnalogInputEntry>(const InputContext::AnalogInputEntry& value)
	{
		return value.device * (value.analog + 1);
	}

	template<>
	FORCE_INLINE UInt32 Hash<InputContext::MouseMoveInputEntry>(const InputContext::MouseMoveInputEntry& value)
	{
		return value.device * (value.axis + 1);
	}

	template<>
	FORCE_INLINE UInt32 Hash<InputContext::MouseButtonInputEntry>(const InputContext::MouseButtonInputEntry& value)
	{
		return value.device * (value.button + 1);
	}

	template<>
	FORCE_INLINE UInt32 Hash<InputContext::MouseWheelInputEntry>(const InputContext::MouseWheelInputEntry& value)
	{
		return value.device;
	}
}