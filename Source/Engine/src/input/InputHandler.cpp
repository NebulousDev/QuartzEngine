#include "InputHandler.h"
#include "../Engine.h"
#include "../log/Log.h"

namespace Quartz
{
	void MouseMoveInputCallback(InputDeviceId deviceId, Int64 relX, Int64 relY)
	{
		Engine& engine = Engine::GetInstanceHandle();
		InputHandler& input = engine.GetInputHandler();

		if (input.mpInputContext->HasMouseMoveBinding(0, MOUSE_X))
		{
			InputBinding binding = input.mpInputContext->GetMouseMoveBinding(0, MOUSE_X);
			input.mInputState.UpdateState(binding, INPUT_NO_ACTION, static_cast<Float32>(relX) * binding.scale);
		}

		if (input.mpInputContext->HasMouseMoveBinding(0, MOUSE_Y))
		{
			InputBinding binding = input.mpInputContext->GetMouseMoveBinding(0, MOUSE_Y);
			input.mInputState.UpdateState(binding, INPUT_NO_ACTION, static_cast<Float32>(relY) * binding.scale);
		}
	}

	void MouseButtonInputCallback(InputDeviceId deviceId, UInt32 button, InputButtonState state)
	{
		Engine& engine = Engine::GetInstanceHandle();
		InputHandler& input = engine.GetInputHandler();

		if (input.mpInputContext->HasMouseButtonBinding(0, button))
		{
			InputBinding binding = input.mpInputContext->GetMouseButtonBinding(0, button);
			InputActionFlags prevActions = input.mInputState.GetStateActionFlags(binding);
			InputActionFlags newActions = INPUT_NO_ACTION;
			Float32 newValue = 0.0f;

			if ((prevActions == INPUT_NO_ACTION || prevActions == INPUT_RELEASED) && state == BUTTON_STATE_DOWN)
			{
				newActions = INPUT_HELD | INPUT_PRESSED;
				newValue = binding.scale;
			}

			else if (prevActions & INPUT_HELD && state == BUTTON_STATE_DOWN)
			{
				newActions = INPUT_HELD | INPUT_REPEATED;
				newValue = binding.scale;
			}

			else if (prevActions & INPUT_HELD && state == BUTTON_STATE_UP)
			{
				newActions = INPUT_RELEASED;
				newValue = 0.0f;
			}

			input.mInputState.UpdateState(binding, newActions, newValue);
		}
	}

	void MouseWheelInputCallback(InputDeviceId deviceId, Float32 value)
	{
		Engine& engine = Engine::GetInstanceHandle();
		InputHandler& input = engine.GetInputHandler();

		if (input.mpInputContext->HasMouseWheelBinding(0))
		{
			InputBinding binding = input.mpInputContext->GetMouseMoveBinding(0, MOUSE_X);
			input.mInputState.UpdateState(binding, INPUT_NO_ACTION, value * binding.scale);
		}
	}

	void KeyboardInputCallback(InputDeviceId deviceId, UInt32 scancode, InputButtonState state)
	{
		Engine& engine = Engine::GetInstanceHandle();
		InputHandler& input = engine.GetInputHandler();

		if (input.mpInputContext->HasKeyboardBinding(0, scancode))
		{
			InputBinding binding = input.mpInputContext->GetKeyboardBinding(0, scancode);
			InputActionFlags prevActions = input.mInputState.GetStateActionFlags(binding);
			InputActionFlags newActions = INPUT_NO_ACTION;
			Float32 newValue = 0.0f;

			if ((prevActions == 0 || prevActions == INPUT_RELEASED) && state == BUTTON_STATE_DOWN)
			{
				newActions = INPUT_HELD | INPUT_PRESSED;
				newValue = binding.scale;
			}

			else if (prevActions & INPUT_HELD && state == BUTTON_STATE_DOWN)
			{
				newActions = INPUT_HELD | INPUT_REPEATED;
				newValue = binding.scale;
			}

			else if (prevActions & INPUT_HELD && state == BUTTON_STATE_UP)
			{
				newActions = INPUT_RELEASED;
				newValue = 0.0f;
			}

			input.mInputState.UpdateState(binding, newActions, newValue);
		}
	}

	void ButtonInputCallback(InputDeviceId deviceId, UInt32 button, InputButtonState state)
	{
		Engine& engine = Engine::GetInstanceHandle();
		InputHandler& input = engine.GetInputHandler();

		if (input.mpInputContext->HasControllerButtonBinding(0, button))
		{
			InputBinding binding = input.mpInputContext->GetControllerButtonBinding(0, button);
			InputActionFlags prevActions = input.mInputState.GetStateActionFlags(binding);
			InputActionFlags newActions = INPUT_NO_ACTION;
			Float32 newValue = 0.0f;

			if ((prevActions == 0 || prevActions == INPUT_RELEASED) && state == BUTTON_STATE_DOWN)
			{
				newActions = INPUT_HELD | INPUT_PRESSED;
				newValue = binding.scale;
			}

			else if (prevActions & INPUT_HELD && state == BUTTON_STATE_DOWN)
			{
				newActions = INPUT_HELD | INPUT_REPEATED;
				newValue = binding.scale;
			}

			else if (prevActions & INPUT_HELD && state == BUTTON_STATE_UP)
			{
				newActions = INPUT_RELEASED;
				newValue = 0.0f;
			}

			input.mInputState.UpdateState(binding, newActions, newValue);
		}
	}

	void InputOnWindowFocusLost(Window& window)
	{
		Engine& engine = Engine::GetInstanceHandle();
		engine.GetInputHandler().ReleaseMouse();
	}

	void InputOnWindowFocusGained(Window& window)
	{
		Engine& engine = Engine::GetInstanceHandle();
		//engine.GetInputHandler().ResumeCaptureMouse();
	}

	InputHandler::InputHandler()
		: mDefaultInputContext("_DefaultInputContext"), 
		mpInputContext(&mDefaultInputContext) {}

	void InputHandler::Init()
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		WindowManager& windowManager = engine.GetWindowManager();

		input.SetMouseMoveInputCallback(MouseMoveInputCallback);
		input.SetMouseButtonInputCallback(MouseButtonInputCallback);
		input.SetMouseWheelInputCallback(MouseWheelInputCallback);
		input.SetKeyboardInputCallback(KeyboardInputCallback);
		input.SetButtonInputCallback(ButtonInputCallback);

		windowManager.SetFocusLostCallback(InputOnWindowFocusLost);
		windowManager.SetFocusGainedCallback(InputOnWindowFocusGained);

		mInputState.GenerateStates(*mpInputContext);
	}

	void InputHandler::Update()
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();

		input.PollInput();

		for (MapPair<InputBinding, InputState::InputBindingState>& boundPair : mInputState.mInputState)
		{
			InputBinding& binding = boundPair.key;
			InputState::InputBindingState& state = boundPair.value;

			if (binding.needsReset && !state.isCurrent)
			{
				state.value = 0.0f;
			}

			if (state.actions & INPUT_PRESSED && !state.isCurrent)
			{
				state.actions = state.actions & ~INPUT_PRESSED;
			}

			else if (state.value != 0.0f)
			{
				Array<InputHandler::InputDispatch*>* pDispatchArray = mDispatches.Get(binding.inputId);

				if (pDispatchArray != nullptr)
				{
					for (InputHandler::InputDispatch* pDispatch : *pDispatchArray)
					{
						pDispatch->DispatchInputCallback(state.actions, state.value);
					}
				}

				state.isCurrent = false;
			}
		}
	}

	void InputHandler::Destroy()
	{
	
	}

	void InputHandler::SetInputContext(InputContext* pContext)
	{
		mpInputContext = pContext;
		mInputState.GenerateStates(*mpInputContext);
	}

	Point2i InputHandler::GetMousePosition() const
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		return input.GetMousePosition();
	}

	void InputHandler::CaptureMouse(Window& window, Bounds2i bounds)
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();

		mpCapturedWindow = &window;
		mCapturedBounds = bounds;
		mMouseCapturePoint = input.GetMousePosition();

		input.HideCursor();
		input.SetCursorBounds(bounds);

		mMouseCaptured = true;
		mMouseCapturePaused = false;
	}

	void InputHandler::PauseCaptureMouse()
	{
		if (mMouseCaptured)
		{
			Engine& engine = Engine::GetInstanceHandle();
			PlatformInput& input = engine.GetPlatform().GetPlatformInput();

			input.SetMousePosition(mMouseCapturePoint);
			input.ShowCursor();
			input.ReleaseCursorBounds();

			mMouseCapturePaused = true;
		}
	}

	void InputHandler::ResumeCaptureMouse()
	{
		if (mMouseCaptured)
		{
			Engine& engine = Engine::GetInstanceHandle();
			PlatformInput& input = engine.GetPlatform().GetPlatformInput();

			mMouseCapturePoint = input.GetMousePosition();
			input.HideCursor();
			input.SetCursorBounds(mCapturedBounds);

			mMouseCapturePaused = false;
		}
	}

	void InputHandler::ReleaseMouse()
	{
		if (mMouseCaptured)
		{
			Engine& engine = Engine::GetInstanceHandle();
			PlatformInput& input = engine.GetPlatform().GetPlatformInput();
			Window& window = engine.GetWindow();

			input.SetMousePosition(mMouseCapturePoint);
			input.ShowCursor();
			input.ReleaseCursorBounds();

			mMouseCaptured = false;
			mMouseCapturePaused = false;
		}
	}

	Bool8 InputHandler::IsMouseCaptured() const
	{
		return mMouseCaptured;
	}

	Bool8 InputHandler::IsMouseCapturePaused() const
	{
		return mMouseCapturePaused;
	}
}

