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

	void InputSystem::PreUpdate(Float32 delta)
	{
		for (auto& actionState : mActionStates)
		{
			if (actionState.value.action == INPUT_ACTION_ACTIVE)
			{
				actionState.value.action = INPUT_ACTION_NONE;
			}
			else if (actionState.value.action == INPUT_ACTION_RELEASED)
			{
				actionState.value.action = actionState.value.action & ~INPUT_ACTION_RELEASED;
			}
		}
	}

	void UpdateActionState(InputActions& actionsState, InputActions actions)
	{
		constexpr UInt32 INPUT_PRESSED_OR_RELEASED = INPUT_ACTION_PRESSED | INPUT_ACTION_RELEASED;

		if (actionsState & INPUT_ACTION_ANY_UP || actionsState == INPUT_ACTION_NONE)
		{
			if (actions & INPUT_ACTION_ANY_DOWN)
			{
				actionsState = INPUT_ACTION_DOWN | INPUT_ACTION_PRESSED;
			}
			else
			{
				actionsState = INPUT_ACTION_UP;
			}
		}
		else if (actionsState & INPUT_ACTION_ANY_DOWN)
		{
			if (actions & INPUT_ACTION_ANY_DOWN)
			{
				actionsState = INPUT_ACTION_DOWN | INPUT_ACTION_REPEAT;
			}
			else
			{
				actionsState = INPUT_ACTION_UP | INPUT_ACTION_RELEASED;
			}
		}
		else
		{
			actionsState = actions;
		}
	}

	void InputSystem::BindKeyboardInputAction(const String& name, Keyboard* pKeyboard, UInt32 key, 
		InputActions actions, const Vector3& axis, Float32 value)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pKeyboard;
		inputKey.type			= PERIPHERAL_KEY_TYPE_KEYBOARD;
		inputKey.id				= key;
		inputKey.actions		= actions;

		InputBinding inputBinding;
		inputBinding.name		= name;
		inputBinding.axis		= axis;
		inputBinding.value		= value;

		mBindings[inputKey].PushBack(inputBinding);
	}

	void InputSystem::BindMouseButtonInputAction(const String& name, Mouse* pMouse, UInt32 button, 
		InputActions actions, const Vector3& axis, Float32 value)
	{
		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_BUTTON;
		inputKey.id				= button;
		inputKey.actions		= actions;

		InputBinding inputBinding;
		inputBinding.name		= name;
		inputBinding.axis		= axis;
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
		inputBinding.axis		= { 0.0f, 0.0f, 0.0f };
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
		inputBinding.axis		= { 0.0f, 0.0f, 0.0f };
		inputBinding.value		= value;

		mBindings[inputKey].PushBack(inputBinding);
	}

	void InputSystem::TriggerKeyboardInputAction(Keyboard* pKeyboard, UInt32 key, InputActions actions)
	{
		InputActions& peripheralState = mStates[(PeripheralHandle)pKeyboard].buttons[key];
		UpdateActionState(peripheralState, actions);

		InputKey inputKey;
		inputKey.pPeripheral	= pKeyboard;
		inputKey.type			= PERIPHERAL_KEY_TYPE_KEYBOARD;
		inputKey.id				= key;
		inputKey.actions		= peripheralState;

		Array<InputBinding>* bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				ActionState& actionsState = mActionStates[binding.name];
				actionsState.axis	= binding.axis;
				actionsState.value	= binding.value;

				UpdateActionState(actionsState.action, actions);

				TriggerInputAction(binding.name, binding.axis, binding.value);
			}
		}

		// Search again with INPUT_ANY_KEYBOARD peripheral to trigger non-specific bindings
		inputKey.pPeripheral = INPUT_ANY_KEYBOARD;

		bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				ActionState& actionsState = mActionStates[binding.name];
				actionsState.axis	= binding.axis;
				actionsState.value	= binding.value;

				UpdateActionState(actionsState.action, actions);

				TriggerInputAction(binding.name, binding.axis, binding.value);
			}
		}
	}

	void InputSystem::TriggerMouseButtonInputAction(Mouse* pMouse, UInt32 button, InputActions actions)
	{
		InputActions& actionsState = mStates[(PeripheralHandle)pMouse].buttons[button];
		UpdateActionState(actionsState, actions);

		InputKey inputKey;
		inputKey.pPeripheral	= pMouse;
		inputKey.type			= PERIPHERAL_KEY_TYPE_MOUSE_BUTTON;
		inputKey.id				= button;
		inputKey.actions		= actionsState;

		Array<InputBinding>* bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				ActionState& actionsState = mActionStates[binding.name];
				actionsState.axis	= binding.axis;
				actionsState.value	= binding.value;

				UpdateActionState(actionsState.action, actions);

				TriggerInputAction(binding.name, binding.axis, binding.value);
			}
		}

		// Search again with INPUT_ANY_MOUSE peripheral to trigger non-specific bindings
		inputKey.pPeripheral = INPUT_ANY_MOUSE;

		bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				ActionState& actionsState = mActionStates[binding.name];
				actionsState.axis	= binding.axis;
				actionsState.value	= binding.value;

				UpdateActionState(actionsState.action, actions);

				TriggerInputAction(binding.name, binding.axis, binding.value);
			}
		}
	}

	void InputSystem::TriggerMouseMoveInputAction(Mouse* pMouse, const Vector2& relative)
	{
		Vector3& axisState = mStates[(PeripheralHandle)pMouse].axis[0];
		Float32 value = relative.Magnitude();
		axisState = Vector3(relative.Normalized(), 0.0f);

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
				ActionState& actionsState = mActionStates[binding.name];
				actionsState.action = INPUT_ACTION_ACTIVE;
				actionsState.axis	= axisState;
				actionsState.value	= value;

				TriggerInputAction(binding.name, axisState, value);
			}
		}

		// Search again with INPUT_ANY_MOUSE peripheral to trigger non-specific bindings
		inputKey.pPeripheral = INPUT_ANY_MOUSE;

		bindings = mBindings.Get(inputKey);

		if (bindings)
		{
			for (InputBinding binding : *bindings)
			{
				ActionState& actionsState = mActionStates[binding.name];
				actionsState.action = INPUT_ACTION_ACTIVE;
				actionsState.axis	= axisState;
				actionsState.value	= value;

				TriggerInputAction(binding.name, axisState, value);
			}
		}
	}

	void InputSystem::TriggerMouseScrollInputAction(Mouse* pMouse, Float32 value, MouseWheelActions actions)
	{

	}

	void InputSystem::TriggerInputAction(const String& name, const Vector3& axis, Float32 value)
	{
		InputActionEvent event;
		event.name		= name;
		event.axis		= axis;
		event.value		= value;

		Engine::GetInstance()->GetEventSystem()->Publish<InputActionEvent>(event);
	}

	Bool8 InputSystem::IsInputActionDown(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			if (pState->action & INPUT_ACTION_ANY_DOWN)
			{
				return true;
			}
		}

		return false;
	}

	Bool8 InputSystem::IsInputActionUp(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			if (pState->action & INPUT_ACTION_ANY_UP)
			{
				return true;
			}
		}

		return false;
	}

	Bool8 InputSystem::IsInputActionPressed(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			if (pState->action & INPUT_ACTION_PRESSED)
			{
				return true;
			}
		}

		return false;
	}

	Bool8 InputSystem::IsInputActionReleased(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			if (pState->action & INPUT_ACTION_RELEASED)
			{
				return true;
			}
		}

		return false;
	}

	Bool8 InputSystem::IsInputActionAxisActive(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			if (pState->action == INPUT_ACTION_ACTIVE)
			{
				return true;
			}
		}

		return false;
	}

	Vector3 InputSystem::GetInputActionAxis(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			return pState->axis;
		}

		return { 0.0f, 0.0f, 0.0f };
	}

	Float32 InputSystem::GetInputActionValue(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			return pState->value;
		}

		return 0.0f;
	}

	ActionState InputSystem::GetInputAction(const String& name)
	{
		const ActionState* pState = mActionStates.Get(name);

		if (pState)
		{
			return *pState;
		}

		return ActionState();
	}
}

