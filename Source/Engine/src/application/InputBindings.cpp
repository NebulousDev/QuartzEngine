#include "InputBindings.h"

namespace Quartz
{
	void InputBindings::BindGlobalMouseMoveCallback(const String& inputName, HVPInputMouse mouse, 
		GlobalMouseMoveCallbackFunc callback, Float32 scaleX, Float32 scaleY)
	{
		mGlobalMouseMoveCallbacks[mouse].PushBack(MouseMoveCallback{ scaleX, scaleY, callback });
	}

	void InputBindings::BindGlobalMouseButtonCallback(const String& inputName, HVPInputMouse mouse, 
		UInt32 button, InputActions actions, GlobalMouseButtonCallbackFunc callback, Float32 scale)
	{
		mGlobalMouseButtonCallbacks[mouse][button].PushBack(MouseButtonCallback{ scale, actions, callback });
	}

	void InputBindings::BindGlobalKeyboardKeyCallback(const String& inputName, HVPInputKeyboard keyboard, 
		UInt32 key, InputActions actions, GlobalKeyboardKeyCallbackFunc callback, Float32 scale)
	{
		mGlobalKeyboardKeyCallbacks[keyboard][key].PushBack(KeyboardKeyCallback{ scale, actions, callback });
	}
}

