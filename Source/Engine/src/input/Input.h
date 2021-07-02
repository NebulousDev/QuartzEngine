#pragma once

#include "InputBindings.h"
#include "InputState.h"

namespace Quartz
{
	class Window;

	class QUARTZ_API Input
	{
	private:
		Array<InputBindings*> mInputBindings;
		InputState mInputState;
		Bool8 mCaptured;

		friend void BindingMouseMoveCallback(HVPInputMouse mouse, Int64 relX, Int64 relY);
		friend void BindingMouseButtonCallback(HVPInputMouse mouse, UInt32 button, ButtonState state);
		friend void BindingKeyboardKeyCallback(HVPInputKeyboard keyboard, UInt32 key, ButtonState state);

		void SendAllMouseMoveCallbacks();
		void SendAllMosueButtonCallbacks();
		void SendAllKeyCallbacks();

		void SendMouseMoveCallbacks(HVPInputMouse mouse, Float32 rx, Float32 ry);
		void SendMouseButtonCallbacks(HVPInputMouse mouse, UInt32 buttonId, InputActions actions);
		void SendKeyCallbacks(HVPInputKeyboard keyboard, UInt32 scancode, InputActions actions);

	public:
		void PreInitialize();
		void Initialize();

		void RegisterInputBindings(InputBindings* pInputBindings);

		void SendMouseMove(HVPInputMouse mouse, Float32 rx, Float32 ry);
		void SendMouseButton(HVPInputMouse mouse, UInt32 buttonId, InputActions actions);
		void SendKey(HVPInputKeyboard keyboard, UInt32 scancode, InputActions actions);
		
		void ShowCursor(Bool8 shown);
		void CaptureCursor(Window& window);
		void ReleaseCursor();

		Point2i GetCursorPosition();
		Bool8 IsMouseCaptured();

		void Update();
	};
}