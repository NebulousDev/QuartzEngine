#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\Map.h"
#include "util\Set.h"
#include "util\String.h"

namespace Quartz
{
	enum QUARTZ_API InputDeviceType
	{
		INPUT_DEVICE_TYPE_KEYBOARD,
		INPUT_DEVICE_TYPE_MOUSE,
		INPUT_DEVICE_TYPE_HID
	};

	enum QUARTZ_API ButtonState : UInt8
	{
		BUTTON_STATE_UP,
		BUTTON_STATE_DOWN,
	};

	/*
	struct QUARTZ_API InputButtonState
	{
		ButtonState buttonValue;
		ButtonState lastButtonValue;
	};

	struct QUARTZ_API InputAnalogState
	{
		Float32 value;
		Float32 lastValue;
		Float32 minValue;
		Float32 maxValue;
	};

	struct QUARTZ_API InputMouseState
	{
		Int32	relativeX;
		Int32	relativeY;
		Int32	absolueX;
		Int32	absolueY;
		Float32	mouseWheel;
	};
	*/

	typedef UInt64 InputDeviceId;

	struct QUARTZ_API InputDeviceDesc
	{
		InputDeviceId	deviceId;
		InputDeviceType deviceType;
		StringW			deviceName;
		StringW			devicePath;
		StringW			serialNuber;
		UInt16			buttonCount;
		UInt16			analogCount;
		UInt16			usagePage;
		UInt16			usage;
		void*			pHandle;
	};

	/*
	struct QUARTZ_API InputDeviceData
	{
		InputMouseState			mouseState;
		Array<InputButtonState> buttonStates;
		Array<InputAnalogState> analogStates;
		Bool8					isConnected;
		Bool8					isDirty;
	};

	struct QUARTZ_API InputDevice
	{
		InputDeviceDescription	desc;
		InputDeviceData			data;
	};
	*/

	typedef void(*InputDeviceConnectCallbackFunc)(InputDeviceId deviceId, InputDeviceDesc deviceDesc);
	typedef void(*InputDeviceDisconnectCallbackFunc)(InputDeviceId deviceId);

	typedef void(*MouseMoveInputCallbackFunc)(InputDeviceId deviceId, Int64 relX, Int64 relY);
	typedef void(*MouseButtonInputCallbackFunc)(InputDeviceId deviceId, UInt32 button, ButtonState state);
	typedef void(*MouseWheelInputCallbackFunc)(InputDeviceId deviceId, Float32 value);
	typedef void(*KeyboardInputCallbackFunc)(InputDeviceId deviceId, UInt32 scancode, ButtonState state);
	typedef void(*ButtonInputCallbackFunc)(InputDeviceId deviceId, UInt32 buttonIdx, ButtonState state);
	typedef void(*AnalogInputCallbackFunc)(InputDeviceId deviceId, UInt32 AnalogIdx, Float32 value);

	class QUARTZ_API PlatformInput
	{
	protected:
		Set<InputDeviceId>					mConnectedDevices;
		Map<InputDeviceId, InputDeviceDesc> mDeviceIdMap;

		InputDeviceConnectCallbackFunc		mDeviceConnectCallbackFunc;
		InputDeviceDisconnectCallbackFunc	mDeviceDisconnectCallbackFunc;
		MouseMoveInputCallbackFunc			mMouseMoveInputCallbackFunc;
		MouseButtonInputCallbackFunc		mMouseButtonInputCallbackFunc;
		MouseWheelInputCallbackFunc			mMouseWheelInputCallbackFunc;
		KeyboardInputCallbackFunc			mKeyboardInputCallbackFunc;
		ButtonInputCallbackFunc				mButtonInputCallbackFunc;
		AnalogInputCallbackFunc				mAnalogInputCallbackFunc;

	public:
		void SetDeviceConnectCallback(InputDeviceConnectCallbackFunc deviceConnectCallback);
		void SetDeviceDisconnectCallback(InputDeviceDisconnectCallbackFunc deviceDisconnectCallback);

		void SetMouseMoveInputCallback(MouseMoveInputCallbackFunc mouseMoveInputCallback);
		void SetMouseButtonInputCallback(MouseButtonInputCallbackFunc mouseButtonInputCallback);
		void SetMouseWheelInputCallback(MouseWheelInputCallbackFunc mouseWheelInputCallbackFunc);
		void SetKeyboardInputCallback(KeyboardInputCallbackFunc keyboardInputCallback);
		void SetButtonInputCallback(ButtonInputCallbackFunc buttonInputCallback);
		void SetAnalogInputCallback(AnalogInputCallbackFunc analogInputCallback);

		virtual void PollDeviceConnections() = 0;
		virtual void PollDeviceInput() = 0;

		Bool8 IsDeviceConnected(InputDeviceId deviceId);
		Bool8 GetDeviceDescription(InputDeviceId deviceId, InputDeviceDesc* pDeviceDesc);
	};
}