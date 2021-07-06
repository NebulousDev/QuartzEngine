#include "InputModule.h"

#include "../Engine.h"

namespace Quartz
{
	#define ACTION_ANY (UInt32)0xFFFF

	template<>
	UInt32 Hash<InputSystem::InputKey>(const InputSystem::InputKey& value)
	{
		return Hash<UInt64>((UInt64)value.pPeripheral + (UInt64)value.type + ((UInt64)value.id << 2));
	}

	InputSystem::InputSystem()
		: Module({ L"Input System" })
	{
		// Nothing
	}

	void InputSystem::BindKeyboardInputAction(const String& name, Keyboard* pKeyboard, UInt32 key, 
		ButtonActions actions, const Vector3& direction, Float32 value)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pKeyboard;
		inputKey.type			= PERIPHERAL_KEY_TYPE_KEYBOARD;
		inputKey.id				= key;
		inputKey.actions		= actions;

		InputBinding inputBinding;
		inputBinding.name		= name;
		inputBinding.direction	= direction;
		inputBinding.value		= value;

		mBindings[inputKey].PushBack(inputBinding);
	}

	void InputSystem::BindMouseButtonInputAction(const String& name, Mouse* pMouse, UInt32 button, 
		ButtonActions actions, const Vector3& direction, Float32 value)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_BUTTON;
		inputKey.id				= button;
		inputKey.actions		= actions;

		InputBinding inputBinding;
		inputBinding.name		= name;
		inputBinding.direction	= direction;
		inputBinding.value		= value;

		mBindings[inputKey].PushBack(inputBinding);
	}

	void InputSystem::BindMouseMoveInputAction(const String& name, Mouse* pMouse)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_MOVE;
		inputKey.id				= 0;
		inputKey.actions		= ACTION_ANY;

		InputBinding inputBinding;
		inputBinding.name		= name;
		inputBinding.direction	= { 0.0f, 0.0f, 0.0f };
		inputBinding.value		= 0.0f;

		mBindings[inputKey].PushBack(inputBinding);
	}

	void InputSystem::BindMouseScrollInputAction(const String& name, Mouse* pMouse, MouseWheelActions actions, Float32 value)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_SCROLL;
		inputKey.id				= 0;
		inputKey.actions		= actions;

		InputBinding inputBinding;
		inputBinding.name		= name;
		inputBinding.direction	= { 0.0f, 0.0f, 0.0f };
		inputBinding.value		= value;

		mBindings[inputKey].PushBack(inputBinding);
	}

	void InputSystem::TriggerKeyboardInputAction(Keyboard* pKeyboard, UInt32 key, ButtonActions actions)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pKeyboard;
		inputKey.type			= PERIPHERAL_KEY_TYPE_KEYBOARD;
		inputKey.id				= key;
		inputKey.actions		= actions;

		Array<InputBinding>* bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				TriggerInputAction(binding.name, binding.direction, binding.value);
			}
		}

		// Search again with INPUT_ANY_KEYBOARD peripheral to trigger non-specific bindings
		inputKey.pPeripheral = INPUT_ANY_KEYBOARD;

		bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				TriggerInputAction(binding.name, binding.direction, binding.value);
			}
		}
	}

	void InputSystem::TriggerMouseButtonInputAction(Mouse* pMouse, UInt32 button, ButtonActions actions)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_BUTTON;
		inputKey.id				= button;
		inputKey.actions		= actions;

		Array<InputBinding>* bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				TriggerInputAction(binding.name, binding.direction, binding.value);
			}
		}

		// Search again with INPUT_ANY_MOUSE peripheral to trigger non-specific bindings
		inputKey.pPeripheral = INPUT_ANY_MOUSE;

		bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				TriggerInputAction(binding.name, binding.direction, binding.value);
			}
		}
	}

	void InputSystem::TriggerMouseMoveInputAction(Mouse* pMouse, const Vector2& relative)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_MOVE;
		inputKey.id				= 0;
		inputKey.actions		= ACTION_ANY;

		Array<InputBinding>* bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				Vector3 direction(relative.Normalized(), 0.0f);
				TriggerInputAction(binding.name, direction, relative.Magnitude());
			}
		}

		// Search again with INPUT_ANY_MOUSE peripheral to trigger non-specific bindings
		inputKey.pPeripheral = INPUT_ANY_MOUSE;

		bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				Vector3 direction(relative.Normalized(), 0.0f);
				TriggerInputAction(binding.name, direction, relative.Magnitude());
			}
		}
	}

	void InputSystem::TriggerMouseScrollInputAction(Mouse* pMouse, Float32 value, MouseWheelActions actions)
	{

	}

	void InputSystem::TriggerInputAction(const String& name, const Vector3& direction, Float32 value)
	{
		InputActionEvent event;
		event.name		= name;
		event.direction = direction;
		event.value		= value;

		Engine::GetInstance()->GetEventSystem()->Publish<InputActionEvent>(event);
	}
}

