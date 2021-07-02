#pragma once

#include "InputAction.h"
#include "../platform/VPPlatform.h"

#include "util/Map.h"

namespace Quartz
{
	typedef void (*GlobalMouseMoveCallbackFunc)(HVPInputMouse mouse, Float32 rx, Float32 ry);
	typedef void (*GlobalMouseButtonCallbackFunc)(HVPInputMouse mouse, UInt32 button, InputActions actions, Float32 value);
	typedef void (*GlobalKeyboardKeyCallbackFunc)(HVPInputKeyboard keyboard, UInt32 key, InputActions actions, Float32 value);

#define ANY_MOUSE reinterpret_cast<HVPInputMouse>(-1)
#define ANY_KEYBOARD reinterpret_cast<HVPInputKeyboard>(-1)
#define ANY_ANALOG reinterpret_cast<HVPInputController>(-1)
#define ANY_BUTTON -1

	class QUARTZ_API InputBindings
	{
	public:
		friend class Input;

	private:
		struct MouseMoveCallback
		{
			Float32 scaleX;
			Float32 scaleY;
			GlobalMouseMoveCallbackFunc callback;
		};

		struct MouseButtonCallback
		{
			Float32 scale;
			InputActions validActions;
			GlobalMouseButtonCallbackFunc callback;
		};

		struct KeyboardKeyCallback
		{
			Float32 scale;
			InputActions validActions;
			GlobalKeyboardKeyCallbackFunc callback;
		};

	private:
		Map<HVPInputMouse, Array<MouseMoveCallback>> mGlobalMouseMoveCallbacks;
		Map<HVPInputMouse, Map<UInt32, Array<MouseButtonCallback>>> mGlobalMouseButtonCallbacks;
		Map<HVPInputKeyboard, Map<UInt32, Array<KeyboardKeyCallback>>> mGlobalKeyboardKeyCallbacks;

	public:
		void BindGlobalMouseMoveCallback(const String& inputName, HVPInputMouse mouse, 
			GlobalMouseMoveCallbackFunc callback, Float32 scaleX, Float32 scaleY);

		void BindGlobalMouseButtonCallback(const String& inputName, HVPInputMouse mouse, 
			UInt32 button, InputActions actions, GlobalMouseButtonCallbackFunc callback, Float32 scale);

		void BindGlobalKeyboardKeyCallback(const String& inputName, HVPInputKeyboard keyboard, 
			UInt32 key, InputActions actions, GlobalKeyboardKeyCallbackFunc callback, Float32 scale);
	};
}