#include "InputState.h"

#include "InputContext.h"

namespace Quartz
{
	InputState::InputState() { }

	void InputState::GenerateStates(InputContext& context)
	{
		mInputState.Clear();

		InputBindingState clearState;
		clearState.actions = INPUT_NO_ACTION;
		clearState.value = 0.0f;
		
		for (const MapPair<InputContext::KeyboardInputEntry, InputBinding>& boundPair : context.mKeyboardInputs)
		{
			mInputState.Put(boundPair.value, clearState);
		}

		for (const MapPair<InputContext::ButtonInputEntry, InputBinding>& boundPair : context.mButtonInputs)
		{
			mInputState.Put(boundPair.value, clearState);
		}

		for (const MapPair<InputContext::AnalogInputEntry, InputBinding>& boundPair : context.mAnalogInputs)
		{
			mInputState.Put(boundPair.value, clearState);
		}

		for (const MapPair<InputContext::MouseMoveInputEntry, InputBinding>& boundPair : context.mMouseMoveInputs)
		{
			mInputState.Put(boundPair.value, clearState);
		}

		for (const MapPair<InputContext::MouseButtonInputEntry, InputBinding>& boundPair : context.mMouseButtonInputs)
		{
			mInputState.Put(boundPair.value, clearState);
		}

		for (const MapPair<InputContext::MouseWheelInputEntry, InputBinding>& boundPair : context.mMouseWheelInputs)
		{
			mInputState.Put(boundPair.value, clearState);
		}
	}

	Bool8 InputState::HasState(InputBinding binding)
	{
		return mInputState.Contains(binding);
	}

	void InputState::UpdateState(InputBinding binding, InputActionFlags newActions, Float32 newValue)
	{
		InputBindingState* pState = mInputState.Get(binding);
		pState->actions = newActions;
		pState->value = newValue;
		pState->isCurrent = true;
	}

	InputActionFlags InputState::GetStateActionFlags(InputBinding binding)
	{
		return mInputState.Get(binding)->actions;
	}

	Float32 InputState::GetStateValue(InputBinding binding)
	{
		return mInputState.Get(binding)->value;
	}
}

