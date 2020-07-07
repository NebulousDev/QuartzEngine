#include "InputSystem.h"
#include "../Engine.h"
#include "../log/Log.h"

namespace Quartz
{
	void MouseMoveInputCallback(InputDeviceId deviceId, Int64 relX, Int64 relY)
	{
		//Log.General("Mouse Move [ID: %lu]: X:%ld, Y:%ld", deviceId, relX, relY);
	}

	void MouseButtonInputCallback(InputDeviceId deviceId, UInt32 button, ButtonState state)
	{
		Log.General("Mouse Button [ID: %lu]: BUTTON: %d, STATE: %s", deviceId, button, state == BUTTON_STATE_DOWN ? "DOWN" : "UP");
	}

	void MouseWheelInputCallback(InputDeviceId deviceId, Float32 value)
	{
		Log.General("Mouse Wheel [ID: %lu]: VALUE: %f", deviceId, value);
	}

	void KeyboardInputCallback(InputDeviceId deviceId, UInt32 scancode, ButtonState state)
	{
		Log.General("Keyboard [ID: %lu]: CODE: %d, STATE: %s", deviceId, scancode, state == BUTTON_STATE_DOWN ? "DOWN" : "UP");
	}

	void ButtonInputCallback(InputDeviceId deviceId, UInt32 button, ButtonState state)
	{
		Log.General("Controller Button [ID: %lu]: BUTTON: %d, STATE: %s", deviceId, button, state == BUTTON_STATE_DOWN ? "DOWN" : "UP");
	}

	void InputSystem::Initialize()
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();

		input.SetMouseMoveInputCallback(MouseMoveInputCallback);
		input.SetMouseButtonInputCallback(MouseButtonInputCallback);
		input.SetMouseWheelInputCallback(MouseWheelInputCallback);
		input.SetKeyboardInputCallback(KeyboardInputCallback);
		input.SetButtonInputCallback(ButtonInputCallback);
	}

	void InputSystem::Update()
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		input.PollDeviceInput();
	}

	void InputSystem::Tick()
	{
		
	}

	void InputSystem::Destroy()
	{
	
	}
}

