#include "Input.h"

#include "../Engine.h"
#include "Window.h"

namespace Quartz
{
	void BindingMouseMoveCallback(HVPInputMouse mouse, Int64 relX, Int64 relY)
	{
		Engine& engine = Engine::GetInstance();
		engine.GetInput().mInputState.UpdateMouseMoveState(mouse, relX, relY);
	}

	void BindingMouseButtonCallback(HVPInputMouse mouse, UInt32 button, ButtonState state)
	{
		Engine& engine = Engine::GetInstance();
		engine.GetInput().mInputState.UpdateMouseButtonState(mouse, button, state);
	}

	void BindingKeyboardKeyCallback(HVPInputKeyboard keyboard, UInt32 key, ButtonState state)
	{
		Engine& engine = Engine::GetInstance();
		Input& input = engine.GetInput();

		Bool8 repeat = input.mInputState.GetKeyAction(keyboard, key) & ACTION_DOWN;
		input.mInputState.UpdateKeyButtonState(keyboard, key, state, repeat);
	}

	void Input::PreInitialize()
	{
		mCaptured = false;
	}

	void Input::Initialize()
	{
		Engine& engine = Engine::GetInstance();
		VPPlatform& platform = engine.GetPlatform();

		platform.SetMouseMoveCallback(BindingMouseMoveCallback);
		platform.SetMouseButtonCallback(BindingMouseButtonCallback);
		platform.SetKeyboardKeyCallback(BindingKeyboardKeyCallback);
	}

	void Input::RegisterInputBindings(InputBindings* pInputBindings)
	{
		mInputBindings.PushBack(pInputBindings);
	}

	void Input::SendMouseMove(HVPInputMouse mouse, Float32 relX, Float32 relY)
	{
		mInputState.UpdateMouseMoveState(mouse, relX, relY);
	}

	void Input::SendMouseButton(HVPInputMouse mouse, UInt32 button, InputActions actions)
	{
		ButtonState state = actions & ANY_UP ? BUTTON_STATE_UP : BUTTON_STATE_DOWN;
		mInputState.UpdateMouseButtonState(mouse, button, state);
	}

	void Input::SendKey(HVPInputKeyboard keyboard, UInt32 key, InputActions actions)
	{
		ButtonState state = actions & ANY_UP ? BUTTON_STATE_UP : BUTTON_STATE_DOWN;
		mInputState.UpdateKeyButtonState(keyboard, key, state, actions & ACTION_REPEAT);
	}

	void Input::ShowCursor(Bool8 shown)
	{
		VPPlatform& platform = Engine::GetInstance().GetPlatform();
		platform.ShowCursor(shown);
	}

	void Input::CaptureCursor(Window& window)
	{
		VPPlatform& platform = Engine::GetInstance().GetPlatform();
		platform.CaptureCursor(window.GetNativeWindow());
	}

	void Input::ReleaseCursor()
	{
		VPPlatform& platform = Engine::GetInstance().GetPlatform();
		platform.ReleaseCursor();
	}

	Point2i Input::GetCursorPosition()
	{
		VPPlatform& platform = Engine::GetInstance().GetPlatform();
		return platform.GetCursorPosition();
	}

	Bool8 Input::IsMouseCaptured()
	{
		VPPlatform& platform = Engine::GetInstance().GetPlatform();
		return platform.GetCapturingWindow() != VP_NULL_HANDLE;
	}

	void Input::SendAllMouseMoveCallbacks()
	{
		for (auto& mouse : mInputState.mMouseMoveStates)
		{
			SendMouseMoveCallbacks(mouse.key, 
				mInputState.GetMouseRelXFromState(mouse.value), 
				mInputState.GetMouseRelYFromState(mouse.value));
		}
	}

	void Input::SendAllMosueButtonCallbacks()
	{
		for (auto& mouse : mInputState.mMouseButtonStates)
		{
			for (auto& button : mouse.value)
			{
				InputActions actions = mInputState.GetMouseButtonActionFromState(button.value);
				SendMouseButtonCallbacks(mouse.key, button.key, actions);
			}
		}
	}

	void Input::SendAllKeyCallbacks()
	{
		for (auto& keyboard : mInputState.mKeyStates)
		{
			for (auto& button : keyboard.value)
			{
				InputActions actions = mInputState.GetKeyActionFromState(button.value);
				SendKeyCallbacks(keyboard.key, button.key, actions);
			}
		}
	}

	void Input::SendMouseMoveCallbacks(HVPInputMouse mouse, Float32 rx, Float32 ry)
	{
		for (InputBindings* pBindings : mInputBindings)
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

	void Input::SendMouseButtonCallbacks(HVPInputMouse mouse, UInt32 button, InputActions actions)
	{
		for (InputBindings* pBindings : mInputBindings)
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

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(mouse, button, actions, 1.0f * callbackObject.scale);

								// We only want to check if there are callbacks for *any* action
								// We break as to not call repeat callbacks
								break;
							}
						}
					}
				}

				if (anyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyButtonCallbacks->Size(); i++)
					{
						InputBindings::MouseButtonCallback& callbackObject = (*anyButtonCallbacks)[i];

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(mouse, button, actions, 1.0f * callbackObject.scale);
								break;
							}
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

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(mouse, button, actions, 1.0f * callbackObject.scale);
								break;
							}
						}
					}
				}

				if (anyMouseAnyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyMouseAnyButtonCallbacks->Size(); i++)
					{
						InputBindings::MouseButtonCallback& callbackObject = (*anyMouseAnyButtonCallbacks)[i];

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(mouse, button, actions, 1.0f * callbackObject.scale);
								break;
							}
						}
					}
				}
			}
		}
	}

	void Input::SendKeyCallbacks(HVPInputKeyboard keyboard, UInt32 key, InputActions actions)
	{
		for (InputBindings* pBindings : mInputBindings)
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

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(keyboard, key, actions, 1.0f * callbackObject.scale);

								// We only want to check if there are callbacks for *any* action
								// We break as to not call repeat callbacks
								break;
							}
						}
					}
				}

				if (anyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyButtonCallbacks->Size(); i++)
					{
						InputBindings::KeyboardKeyCallback& callbackObject = (*anyButtonCallbacks)[i];

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(keyboard, key, actions, 1.0f * callbackObject.scale);
								break;
							}
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

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(keyboard, key, actions, 1.0f * callbackObject.scale);
								break;
							}
						}
					}
				}

				if (anyMouseAnyButtonCallbacks != nullptr)
				{
					for (UInt32 i = 0; i < anyMouseAnyButtonCallbacks->Size(); i++)
					{
						InputBindings::KeyboardKeyCallback& callbackObject = (*anyMouseAnyButtonCallbacks)[i];

						for (UInt32 i = 0x1; i <= (UInt32)(ACTION_REPEAT); i <<= 1)
						{
							if (actions & i && callbackObject.validActions & i)
							{
								callbackObject.callback(keyboard, key, actions, 1.0f * callbackObject.scale);
								break;
							}
						}
					}
				}
			}
		}
	}
	
	void Input::Update()
	{
		if (true);//mCaptured)
		{
			SendAllMouseMoveCallbacks();
			SendAllMosueButtonCallbacks();
			SendAllKeyCallbacks();

			mInputState.UpdateStates();
		}
	}
}

