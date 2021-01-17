#pragma once
#include "VPPlatform.h"

namespace Quartz
{
	/* Virtual Platform Objects */

	struct VPInputMouse : public VPResource {};
	struct VPInputKeyboard : public VPResource {};
	struct VPInputController : public VPResource {};

	/* Virtual Platform Handles */

	typedef VPResource* HVPInputMouse;
	typedef VPResource* HVPInputKeyboard;
	typedef VPResource* HVPInputController;

	/* Enums */

	enum ButtonState : UInt8
	{
		BUTTON_STATE_UP = 0x0,
		BUTTON_STATE_DOWN = 0x2
	};

	typedef Flags8 ButtonStates;

	/* Virtual Platform Callback Declarations */

	//typedef void(*InputDeviceConnectCallbackFunc)(InputDeviceId deviceId, InputDeviceInfo deviceDesc);
	//typedef void(*InputDeviceDisconnectCallbackFunc)(InputDeviceId deviceId);

	typedef void(*VPMouseMoveCallbackFunc)(HVPInputMouse mouse, Int64 relX, Int64 relY);
	typedef void(*VPMouseButtonCallbackFunc)(HVPInputMouse mouse, UInt32 button, ButtonState state);
	//typedef void(*VPMouseWheelInputCallbackFunc)(VPInputMouse deviceId, Float32 value);
	typedef void(*VPKeyboardKeyCallbackFunc)(HVPInputKeyboard keyboard, UInt32 scancode, ButtonState state);
	//typedef void(*ButtonInputCallbackFunc)(InputDeviceId deviceId, UInt32 button, InputButtonState state);
	//typedef void(*AnalogInputCallbackFunc)(InputDeviceId deviceId, UInt32 analog, Float32 value);

	class QUARTZ_API VPInput
	{
	public:
		virtual void PreInitialize() = 0;
		virtual void Initialize() = 0;

		//void SetDeviceConnectCallback(InputDeviceConnectCallbackFunc deviceConnectCallback);
		//void SetDeviceDisconnectCallback(InputDeviceDisconnectCallbackFunc deviceDisconnectCallback);

		virtual void SetMouseMoveCallback(VPMouseMoveCallbackFunc callback) = 0;
		virtual void SetMouseButtonCallback(VPMouseButtonCallbackFunc callback) = 0;
		//void SetMouseWheelInputCallback(VPMouseWheelInputCallbackFunc mouseWheelInputCallbackFunc);
		virtual void SetKeyboardKeyCallback(VPKeyboardKeyCallbackFunc callback) = 0;
		//void SetButtonInputCallback(ButtonInputCallbackFunc buttonInputCallback);
		//void SetAnalogInputCallback(AnalogInputCallbackFunc analogInputCallback);

		virtual void PollConnections() = 0;
		virtual void PollInput() = 0;
	};

	template<>
	FORCE_INLINE UInt32 Hash<VPResource*>(const HVPInputMouse& value)
	{
		return Hash<UInt64>(reinterpret_cast<UInt64>(value));
	}
}