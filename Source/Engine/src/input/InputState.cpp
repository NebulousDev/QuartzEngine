#include "InputState.h"

namespace Quartz
{
	Float32 InputState::GetMouseRelXFromState(MouseMoveState& state)
	{
		return state.relX;
	}

	Float32 InputState::GetMouseRelYFromState(MouseMoveState& state)
	{
		return state.relY;
	}

	InputActions InputState::GetMouseButtonActionFromState(MouseButtonState& state)
	{
		if (state.state & BUTTON_STATE_DOWN)
		{
			if (state.lastState == BUTTON_STATE_DOWN)
			{
				return ACTION_DOWN;
			}
			else
			{
				return ACTION_DOWN | ACTION_PRESSED;
			}
		}

		else
		{
			if (state.lastState == BUTTON_STATE_DOWN)
			{
				return ACTION_UP | ACTION_RELEASED;
			}
			else
			{
				return ACTION_UP;
			}
		}
	}

	InputActions InputState::GetKeyActionFromState(KeyState& state)
	{
		InputActions actions = 0;

		if (state.state & BUTTON_STATE_DOWN)
		{
			if (state.lastState == BUTTON_STATE_DOWN)
			{
				actions |= ACTION_DOWN;
			}
			else
			{
				actions |= (ACTION_DOWN | ACTION_PRESSED);
			}
		}

		else
		{
			if (state.lastState == BUTTON_STATE_DOWN)
			{
				actions |= (ACTION_UP | ACTION_RELEASED);
			}
			else
			{
				actions |= ACTION_UP;
			}
		}

		if (state.repeat)
		{
			actions |= ACTION_REPEAT;
		}

		return actions;
	}

	void InputState::UpdateMouseMoveState(HVPInputMouse mouse, Float32 relX, Float32 relY)
	{
		MouseMoveState& state = mMouseMoveStates[mouse];
		state.relX = relX;
		state.relY = relY;
	}

	void InputState::UpdateMouseButtonState(HVPInputMouse mouse, UInt32 button, ButtonState newState)
	{
		MouseButtonState& state = mMouseButtonStates[mouse][button];
		state.state = newState;
	}

	void InputState::UpdateKeyButtonState(HVPInputKeyboard keyboard, UInt32 key, ButtonState newState, Bool8 repeat)
	{
		KeyState& state = mKeyStates[keyboard][key];
		state.state = newState;
		state.repeat = repeat;
	}

	void InputState::UpdateStates()
	{
		for (auto& mouse : mMouseMoveStates)
		{
			MouseMoveState& state = mouse.value;
			state.lastRelX = state.relX;
			state.lastRelY = state.relY;
			state.relX = 0.0f;
			state.relY = 0.0f;
		}

		for (auto& mouseButtons : mMouseButtonStates)
		{
			for (auto& mouseState : mouseButtons.value)
			{
				MouseButtonState& state = mouseState.value;
				state.lastState = state.state;
			}
		}

		for (auto& keyboard : mKeyStates)
		{
			for (auto& keyState : keyboard.value)
			{
				KeyState& state = keyState.value;
				state.lastState = state.state;
				state.repeat = false;
			}
		}
	}

	Float32 InputState::GetMouseRelX(HVPInputMouse mouse)
	{
		return GetMouseRelXFromState(mMouseMoveStates[mouse]);
	}

	Float32 InputState::GetMouseRelY(HVPInputMouse mouse)
	{
		return GetMouseRelYFromState(mMouseMoveStates[mouse]);
	}

	InputActions InputState::GetMouseButtonAction(HVPInputMouse mouse, UInt32 button)
	{
		MouseButtonState& state = mMouseButtonStates[mouse][button];
		return GetMouseButtonActionFromState(state);
	}

	InputActions InputState::GetKeyAction(HVPInputKeyboard keyboard, UInt32 key)
	{
		KeyState& state = mKeyStates[keyboard][key];
		return GetKeyActionFromState(state);
	}
}

