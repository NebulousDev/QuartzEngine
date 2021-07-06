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
			ButtonActions		actions;

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
			Vector3 direction;
			Float32 value;
		};

	private:
		Map<InputKey, Array<InputBinding>> mBindings;

	public:
		InputSystem();

		void BindKeyboardInputAction(const String& name, Keyboard* pKeyboard, UInt32 key, 
			ButtonActions actions, const Vector3& direction, Float32 value);
		void BindMouseButtonInputAction(const String& name, Mouse* pMouse, UInt32 button, 
			ButtonActions actions, const Vector3& direction, Float32 value);
		void BindMouseMoveInputAction(const String& name, Mouse* pMouse);
		void BindMouseScrollInputAction(const String& name, Mouse* pMouse, 
			MouseWheelActions actions, Float32 value);

		void TriggerKeyboardInputAction(Keyboard* pKeyboard, UInt32 key, ButtonActions actions);
		void TriggerMouseButtonInputAction(Mouse* pMouse, UInt32 button, ButtonActions actions);
		void TriggerMouseMoveInputAction(Mouse* pMouse, const Vector2& relative);
		void TriggerMouseScrollInputAction(Mouse* pMouse, Float32 value, MouseWheelActions actions);

		void TriggerInputAction(const String& name, const Vector3& direction, Float32 value);
	};
}