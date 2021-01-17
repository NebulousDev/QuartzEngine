#pragma once

#include "InputBindings.h"
#include "InputState.h"

namespace Quartz
{
	class QUARTZ_API Input
	{
	private:
		static Array<InputBindings*> smInputBindings;
		static InputState smInputState;

		friend void BindingMouseMoveCallback(HVPInputMouse mouse, Int64 relX, Int64 relY);
		friend void BindingMouseButtonCallback(HVPInputMouse mouse, UInt32 button, ButtonState state);
		friend void BindingKeyboardKeyCallback(HVPInputKeyboard keyboard, UInt32 key, ButtonState state);

		static void SendAllMouseMoveCallbacks();
		static void SendAllMosueButtonCallbacks();
		static void SendAllKeyCallbacks();

		static void SendMouseMoveCallbacks(HVPInputMouse mouse, Float32 rx, Float32 ry);
		static void SendMouseButtonCallbacks(HVPInputMouse mouse, UInt32 buttonId, InputActions actions);
		static void SendKeyCallbacks(HVPInputKeyboard keyboard, UInt32 scancode, InputActions actions);

	public:
		static void PreInitialize();
		static void Initialize(VPInput* pInput);

		static void RegisterInputBindings(InputBindings* pInputBindings);

		static void SendMouseMove(HVPInputMouse mouse, Float32 rx, Float32 ry);
		static void SendMouseButton(HVPInputMouse mouse, UInt32 buttonId, InputActions actions);
		static void SendKey(HVPInputKeyboard keyboard, UInt32 scancode, InputActions actions);

		static void Update();
	};
}