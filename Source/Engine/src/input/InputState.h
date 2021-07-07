#pragma once

#include "InputAction.h"
#include "../platform/VPPlatform.h"

#include "util/Map.h"

namespace Quartz
{
	class QUARTZ_API InputState
	{
	public:
		friend class InputSystem;

	private:
		struct MouseMoveState
		{
			Float32 relX;
			Float32 relY;
			Float32 lastRelX;
			Float32 lastRelY;
		};

		struct MouseButtonState
		{
			ButtonState state;
			ButtonState lastState;
		};

		struct KeyState
		{
			ButtonState state;
			ButtonState lastState;
			Bool8 repeat;
		};

		Map<HVPInputMouse, MouseMoveState> mMouseMoveStates;
		Map<HVPInputMouse, Map<UInt32, MouseButtonState>> mMouseButtonStates;
		Map<HVPInputKeyboard, Map<UInt32, KeyState>> mKeyStates;

	private:
		Float32 GetMouseRelXFromState(MouseMoveState& state);
		Float32 GetMouseRelYFromState(MouseMoveState& state);
		InputActions GetMouseButtonActionFromState(MouseButtonState& state);
		InputActions GetKeyActionFromState(KeyState& state);

	public:
		void UpdateMouseMoveState(HVPInputMouse mouse, Float32 relX, Float32 relY);
		void UpdateMouseButtonState(HVPInputMouse mouse, UInt32 button, ButtonState state);
		void UpdateKeyButtonState(HVPInputKeyboard keyboard, UInt32 key, ButtonState state, Bool8 repeat);

		void UpdateStates();
		
		Float32 GetMouseRelX(HVPInputMouse mouse);
		Float32 GetMouseRelY(HVPInputMouse mouse);
		InputActions GetMouseButtonAction(HVPInputMouse mouse, UInt32 button);
		InputActions GetKeyAction(HVPInputKeyboard keyboard, UInt32 key);
	};
}