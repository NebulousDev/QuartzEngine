#include "InputContext.h"

namespace Quartz
{
	InputContext::InputContext(String name)
		: mName(name) { }

	void InputContext::BindKeyboardInput(String inputName, InputDeviceId device, UInt32 scancode, Float32 scale)
	{
		KeyboardInputEntry entry;
		entry.device = device;
		entry.scancode = scancode;

		InputBinding binding;
		binding.inputId = inputName.Hash();
		binding.scale = scale;
		binding.needsReset = false;

		mKeyboardInputs.Put(entry, binding);
	}

	void InputContext::BindControllerButtonInput(String inputName, InputDeviceId device, UInt32 button, Float32 scale)
	{
		ButtonInputEntry entry;
		entry.device = device;
		entry.button = button;

		InputBinding binding;
		binding.inputId = inputName.Hash();
		binding.scale = scale;
		binding.needsReset = false;

		mButtonInputs.Put(entry, binding);
	}

	void InputContext::BindControllerAnalogInput(String inputName, InputDeviceId device, UInt32 analog, Float32 scale)
	{
		AnalogInputEntry entry;
		entry.device = device;
		entry.analog = analog;

		InputBinding binding;
		binding.inputId = inputName.Hash();
		binding.scale = scale;
		binding.needsReset = false;

		mAnalogInputs.Put(entry, binding);
	}

	void InputContext::BindMouseMoveInput(String inputName, InputDeviceId device, UInt32 axis, Float32 scale)
	{
		MouseMoveInputEntry entry;
		entry.device = device;
		entry.axis = axis;

		InputBinding binding;
		binding.inputId = inputName.Hash();
		binding.scale = scale;
		binding.needsReset = true;

		mMouseMoveInputs.Put(entry, binding);
	}

	void InputContext::BindMouseButtonInput(String inputName, InputDeviceId device, UInt32 button, Float32 scale)
	{
		MouseButtonInputEntry entry;
		entry.device = device;
		entry.button = button;

		InputBinding binding;
		binding.inputId = inputName.Hash();
		binding.scale = scale;
		binding.needsReset = false;

		mMouseButtonInputs.Put(entry, binding);
	}

	void InputContext::BindMouseWheelInput(String inputName, InputDeviceId device, Float32 scale)
	{
		MouseWheelInputEntry entry;
		entry.device = device;

		InputBinding binding;
		binding.inputId = inputName.Hash();
		binding.scale = scale;
		binding.needsReset = true;

		mMouseWheelInputs.Put(entry, binding);
	}

	InputBinding InputContext::GetKeyboardBinding(InputDeviceId device, UInt32 scancode)
	{
		KeyboardInputEntry entry;
		entry.device = device;
		entry.scancode = scancode;

		InputBinding* pInputBinding = mKeyboardInputs.Get(entry);

		if (pInputBinding != nullptr)
		{
			return *pInputBinding;
		}

		return InputBinding();
	}

	InputBinding InputContext::GetControllerButtonBinding(InputDeviceId device, UInt32 button)
	{
		ButtonInputEntry entry;
		entry.device = device;
		entry.button = button;

		InputBinding* pInputBinding = mButtonInputs.Get(entry);

		if (pInputBinding != nullptr)
		{
			return *pInputBinding;
		}

		return InputBinding();
	}

	InputBinding InputContext::GetControllerAnalogBinding(InputDeviceId device, UInt32 analog)
	{
		AnalogInputEntry entry;
		entry.device = device;
		entry.analog = analog;

		InputBinding* pInputBinding = mAnalogInputs.Get(entry);

		if (pInputBinding != nullptr)
		{
			return *pInputBinding;
		}

		return InputBinding();
	}

	InputBinding InputContext::GetMouseMoveBinding(InputDeviceId device, UInt32 axis)
	{
		MouseMoveInputEntry entry;
		entry.device = device;
		entry.axis = axis;

		InputBinding* pInputBinding = mMouseMoveInputs.Get(entry);

		if (pInputBinding != nullptr)
		{
			return *pInputBinding;
		}

		return InputBinding();
	}

	InputBinding InputContext::GetMouseButtonBinding(InputDeviceId device, UInt32 button)
	{
		MouseButtonInputEntry entry;
		entry.device = device;
		entry.button = button;

		InputBinding* pInputBinding = mMouseButtonInputs.Get(entry);

		if (pInputBinding != nullptr)
		{
			return *pInputBinding;
		}

		return InputBinding();
	}

	InputBinding InputContext::GetMouseWheelBinding(InputDeviceId device)
	{
		MouseWheelInputEntry entry;
		entry.device = device;

		InputBinding* pInputBinding = mMouseWheelInputs.Get(entry);

		if (pInputBinding != nullptr)
		{
			return *pInputBinding;
		}

		return InputBinding();
	}

	Bool8 InputContext::HasKeyboardBinding(InputDeviceId device, UInt32 scancode)
	{
		KeyboardInputEntry entry;
		entry.device = device;
		entry.scancode = scancode;

		return mKeyboardInputs.Contains(entry);
	}

	Bool8 InputContext::HasControllerButtonBinding(InputDeviceId device, UInt32 button)
	{
		ButtonInputEntry entry;
		entry.device = device;
		entry.button = button;

		return mButtonInputs.Contains(entry);
	}

	Bool8 InputContext::HasControllerAnalogBinding(InputDeviceId device, UInt32 analog)
	{
		AnalogInputEntry entry;
		entry.device = device;
		entry.analog = analog;

		return mAnalogInputs.Contains(entry);
	}

	Bool8 InputContext::HasMouseMoveBinding(InputDeviceId device, UInt32 axis)
	{
		MouseMoveInputEntry entry;
		entry.device = device;
		entry.axis = axis;

		return mMouseMoveInputs.Contains(entry);
	}

	Bool8 InputContext::HasMouseButtonBinding(InputDeviceId device, UInt32 button)
	{
		MouseButtonInputEntry entry;
		entry.device = device;
		entry.button = button;

		return mMouseButtonInputs.Contains(entry);
	}

	Bool8 InputContext::HasMouseWheelBinding(InputDeviceId device)
	{
		MouseWheelInputEntry entry;
		entry.device = device;

		return mMouseWheelInputs.Contains(entry);
	}
}