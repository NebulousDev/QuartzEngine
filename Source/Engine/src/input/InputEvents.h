#pragma once

#include "../event/Event.h"
#include "Peripherals.h"
#include "InputAction.h"
#include "math/Math.h"
#include "util/String.h"

namespace Quartz
{
	struct RawKeyEvent : public Event<RawKeyEvent>
	{
		Keyboard*		pKeyboard;
		UInt32			key;
		ButtonAction	action;
	};

	struct RawMouseButtonEvent : public Event<RawMouseButtonEvent>
	{
		Mouse*			pMouse;
		UInt32			button;
		ButtonAction	action;
	};

	struct RawMouseMoveEvent : public Event<RawMouseMoveEvent>
	{
		Mouse*	pMouse;
		Vector2	relative;
		Vector2 absolute;
	};

	struct InputActionEvent : public Event<InputActionEvent>
	{
		String	name;
		Vector3 direction;
		Float32 value;
	};
}