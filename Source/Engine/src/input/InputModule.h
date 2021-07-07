#pragma once

#include "../Module.h"
#include "Peripherals.h"
#include "InputEvents.h"

#include "util/Array.h"
#include "util/Map.h"

namespace Quartz
{
	#define INPUT_ANY_KEYBOARD		nullptr
	#define INPUT_ANY_MOUSE			nullptr
	#define INPUT_ANY_CONTROLLER	nullptr

	#define INPUT_MAX_PERIPHERAL_BUTTONS	128
	#define INPUT_MAX_PERIPHERAL_AXIS		8

	struct QUARTZ_API ActionState
	{
		InputActions	action;
		Vector3			axis;
		Float32			value;
	};

	class QUARTZ_API InputSystem : public Module
	{
	private:
		enum PeripheralKeyType
		{
			PERIPHERAL_KEY_TYPE_KEYBOARD,
			PERIPHERAL_KEY_TYPE_MOUSE_BUTTON,
			PERIPHERAL_KEY_TYPE_MOUSE_MOVE,
			PERIPHERAL_KEY_TYPE_MOUSE_SCROLL,
			PERIPHERAL_KEY_TYPE_CONTROLLER_BUTTON,
			PERIPHERAL_KEY_TYPE_CONTROLLER_JOYSTICK
		};

		struct InputKey
		{
			Peripheral*			pPeripheral;
			PeripheralKeyType	type;
			UInt32				id;
			InputActions		actions;

			FORCE_INLINE Bool8 operator==(const InputKey& key)
			{
				// Only one action needs to be present to pass the hash lookup
				return 
					pPeripheral == key.pPeripheral && 
					type == key.type && 
					id == key.id &&
					(actions & key.actions);
			}
		};

		struct InputBinding
		{
			String	name;
			Vector3 axis;
			Float32 value;
		};

		struct PeripheralState
		{
			InputActions	buttons[INPUT_MAX_PERIPHERAL_BUTTONS];
			Vector3			axis[INPUT_MAX_PERIPHERAL_AXIS];
		};

		typedef Handle64 PeripheralHandle;

	private:
		Map<InputKey, Array<InputBinding>>		mBindings;
		Map<PeripheralHandle, PeripheralState>	mStates;
		Map<String, ActionState>				mActionStates;

	public:
		InputSystem();

		void PreUpdate(Float32 delta) override;

		void BindKeyboardInputAction(const String& name, Keyboard* pKeyboard, UInt32 key, 
			InputActions actions, const Vector3& axis, Float32 value);
		void BindMouseButtonInputAction(const String& name, Mouse* pMouse, UInt32 button, 
			InputActions actions, const Vector3& axis, Float32 value);
		void BindMouseMoveInputAction(const String& name, Mouse* pMouse);
		void BindMouseScrollInputAction(const String& name, Mouse* pMouse, 
			MouseWheelActions actions, Float32 value);

		void TriggerKeyboardInputAction(Keyboard* pKeyboard, UInt32 key, InputActions actions);
		void TriggerMouseButtonInputAction(Mouse* pMouse, UInt32 button, InputActions actions);
		void TriggerMouseMoveInputAction(Mouse* pMouse, const Vector2& relative);
		void TriggerMouseScrollInputAction(Mouse* pMouse, Float32 value, MouseWheelActions actions);

		void TriggerInputAction(const String& name, const Vector3& axis, Float32 value);

		Bool8 IsInputActionDown(const String& name);
		Bool8 IsInputActionUp(const String& name);
		Bool8 IsInputActionPressed(const String& name);
		Bool8 IsInputActionReleased(const String& name);

		Bool8 IsInputActionAxisActive(const String& name);
		Vector3 GetInputActionAxis(const String& name);
		Float32 GetInputActionValue(const String& name);

		ActionState GetInputAction(const String& name);
	};
}