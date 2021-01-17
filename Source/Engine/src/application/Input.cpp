#include "Input.h"

namespace Quartz
{
	Array<InputBindings*> Input::smInputBindings;
	InputState Input::smInputState;

	void BindingMouseMoveCallback(HVPInputMouse mouse, Int64 relX, Int64 relY)
	{
		Input::smInputState.UpdateMouseMoveState(mouse, relX, relY);
	}

	void BindingMouseButtonCallback(HVPInputMouse mouse, UInt32 button, ButtonState state)
	{
		Input::smInputState.UpdateMouseButtonState(mouse, button, state);
	}

	void BindingKeyboardKeyCallback(HVPInputKeyboard keyboard, UInt32 key, ButtonState state)
	{
		Bool8 repeat = Input::smInputState.GetKeyAction(keyboard, key) & ACTION_DOWN;
		Input::smInputState.UpdateKeyButtonState(keyboard, key, state, repeat);
	}

	void Input::PreInitialize()
	{

	}

	void Input::Initialize(VPInput* pInput)
	{
		pInput->SetMouseMoveCallback(BindingMouseMoveCallback);
		pInput->SetMouseButtonCallback(BindingMouseButtonCallback);
		pInput->SetKeyboardKeyCallback(BindingKeyboardKeyCallback);
	}

	void Input::RegisterInputBindings(InputBindings* pInputBindings)
	{
		smInputBindings.PushBack(pInputBindings);
	}

	void Input::SendMouseMove(HVPInputMouse mouse, Float32 relX, Float32 relY)
	{
		Input::smInputState.UpdateMouseMoveState(mouse, relX, relY);
	}

	void Input::SendMouseButton(HVPInputMouse mouse, UInt32 button, InputAction action)
	{
		ButtonState state = action & ANY_UP ? BUTTON_STATE_UP : BUTTON_STATE_DOWN;
		Input::smInputState.UpdateMouseButtonState(mouse, button, state);
	}

	void Input::SendKey(HVPInputKeyboard keyboard, UInt32 key, InputAction action)
	{
		ButtonState state = action & ANY_UP ? BUTTON_STATE_UP : BUTTON_STATE_DOWN;
		Input::smInputState.UpdateKeyButtonState(keyboard, key, state, action & ACTION_REPEAT);
	}

	void Input::SendAllMouseMoveCallbacks()
	{
		for (auto& mouse : smInputState.mMouseMoveStates)
		{
			SendMouseMoveCallbacks(mouse.key, 
				smInputState.GetMouseRelXFromState(mouse.value), 
				smInputState.GetMouseRelYFromState(mouse.value));
		}
	}

	void Input::SendAllMosueButtonCallbacks()
	{
		for (auto& mouse : smInputState.mMouseButtonStates)
		{
			for (auto& button : mouse.value)
			{
				InputActions actions = smInputState.GetMouseButtonActionFromState(button.value);

				for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
				{
					if (actions & i)
					{
						InputAction action = (InputAction)i;
						SendMouseButtonCallbacks(mouse.key, button.key, action);

						// We only want to check if there are callbacks for *any* action
						// We break as to not call repeat callbacks
						break;
					}
				}
			}

		}
	}

	void Input::SendAllKeyCallbacks()
	{
		for (auto& keyboard : smInputState.mKeyStates)
		{
			for (auto& button : keyboard.value)
			{
				InputActions actions = smInputState.GetKeyActionFromState(button.value);

				for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT << 1); i <<= 1)
				{
					if (actions & i)
					{
						InputAction action = (InputAction)i;
						SendKeyCallbacks(keyboard.key, button.key, action);

						// We only want to check if there are callbacks for *any* action
						// We break as to not call repeat callbacks
						break;
					}
				}
			}

		}
	}

	void Input::SendMouseMoveCallbacks(HVPInputMouse mouse, Float32 rx, Float32 ry)
	{
		for (InputBindings* pBindings : smInputBindings)
		{
			auto* callbacks = pBindings->mGlobalMouseMoveCallbacks.Get(mouse);
			auto* anyCallbacks = pBindings->mGlobalMouseMoveCallbacks.Get(ANY_MOUSE);

			if (callbacks != nullptr)
			{
				for (UInt32 i = 0; i < callbacks->Size(); i++)
				{
					InputBindings::MouseMoveCallback& callbackObject = (*callbacks)[i];

					callbackObject.callback(mouse,
						(Float32)rx * callbackObject.scaleX,
						(Float32)ry * callbackObject.scaleY);
				}
			}

			if (anyCallbacks != nullptr)
			{
				for (UInt32 i = 0; i < anyCallbacks->Size(); i++)
				{
					InputBindings::MouseMoveCallback& callbackObject = (*anyCallbacks)[i];

					callbackObject.callback(mouse,
						(Float32)rx * callbackObject.scaleX,
						(Float32)ry * callbackObject.scaleY);
				}
			}
		}
	}

	void Input::SendMouseButtonCallbacks(HVPInputMouse mouse, UInt32 button, InputAction action)
	{
		for (InputBindings* pBindings : smInputBindings)
		{
			auto* callbackMaps = pBindings->mGlobalMouseButtonCallbacks.Get(mouse);
			auto* anyMouseCallbackMaps = pBindings->mGlobalMouseButtonCallbacks.Get(ANY_MOUSE);

			if (callbackMaps != nullptr)
			{
				auto* callbacks = callbackMaps->Get(button);
				auto* anyButtonCallbacks = callbackMaps->Get(ANY_BUTTON);

				if (callbacks != nullptr)
				{
					for (UInt32 i = 0; i < callbacks->Size(); i++)
					{
						InputBindings::MouseButtonCallback& callbackObject = (*callbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(mouse, button, action, 1.0f * callbackObject.scale);
						}
					}
				}

				if (anyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyButtonCallbacks->Size(); i++)
					{
						InputBindings::MouseButtonCallback& callbackObject = (*anyButtonCallbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(mouse, button, action, 1.0f * callbackObject.scale);
						}
					}
				}

			}

			if (anyMouseCallbackMaps != nullptr)
			{
				auto* anyMouseCallbacks = anyMouseCallbackMaps->Get(button);
				auto* anyMouseAnyButtonCallbacks = anyMouseCallbackMaps->Get(ANY_BUTTON);

				if (anyMouseCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyMouseCallbacks->Size(); i++)
					{
						InputBindings::MouseButtonCallback& callbackObject = (*anyMouseCallbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(mouse, button, action, 1.0f * callbackObject.scale);
						}
					}
				}

				if (anyMouseAnyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyMouseAnyButtonCallbacks->Size(); i++)
					{
						InputBindings::MouseButtonCallback& callbackObject = (*anyMouseAnyButtonCallbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(mouse, button, action, 1.0f * callbackObject.scale);
						}
					}
				}
			}
		}
	}

	void Input::SendKeyCallbacks(HVPInputKeyboard keyboard, UInt32 key, InputAction action)
	{
		for (InputBindings* pBindings : smInputBindings)
		{
			auto* callbackMaps = pBindings->mGlobalKeyboardKeyCallbacks.Get(keyboard);
			auto* anyMouseCallbackMaps = pBindings->mGlobalKeyboardKeyCallbacks.Get(ANY_MOUSE);

			if (callbackMaps != nullptr)
			{
				auto* callbacks = callbackMaps->Get(key);
				auto* anyButtonCallbacks = callbackMaps->Get(ANY_BUTTON);

				if (callbacks != nullptr)
				{
					for (UInt32 i = 0; i < callbacks->Size(); i++)
					{
						InputBindings::KeyboardKeyCallback& callbackObject = (*callbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(keyboard, key, action, 1.0f * callbackObject.scale);
						}
					}
				}

				if (anyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyButtonCallbacks->Size(); i++)
					{
						InputBindings::KeyboardKeyCallback& callbackObject = (*anyButtonCallbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(keyboard, key, action, 1.0f * callbackObject.scale);
						}
					}
				}

			}

			if (anyMouseCallbackMaps != nullptr)
			{
				auto* anyMouseCallbacks = anyMouseCallbackMaps->Get(key);
				auto* anyMouseAnyButtonCallbacks = anyMouseCallbackMaps->Get(ANY_BUTTON);

				if (anyMouseCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyMouseCallbacks->Size(); i++)
					{
						InputBindings::KeyboardKeyCallback& callbackObject = (*anyMouseCallbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(keyboard, key, action, 1.0f * callbackObject.scale);
						}
					}
				}

				if (anyMouseAnyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyMouseAnyButtonCallbacks->Size(); i++)
					{
						InputBindings::KeyboardKeyCallback& callbackObject = (*anyMouseAnyButtonCallbacks)[i];

						if (callbackObject.validActions & action)
						{
							callbackObject.callback(keyboard, key, action, 1.0f * callbackObject.scale);
						}
					}
				}
			}
		}
	}
	
	void Input::Update()
	{
		SendAllMouseMoveCallbacks();
		SendAllMosueButtonCallbacks();
		SendAllKeyCallbacks();

		smInputState.UpdateStates();
	}
}

