#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\Map.h"
#include "util\Set.h"
#include "util\String.h"
#include "application\Window.h"

namespace Quartz
{
	/*
		The type of and input device
		
		INPUT_DEVICE_TYPE_KEYBOARD - keyboard input devices
		INPUT_DEVICE_TYPE_MOUSE - mouse input devices
		INPUT_DEVICE_TYPE_HID - controllers, special input devices
	*/
	enum QUARTZ_API InputDeviceType
	{
		INPUT_DEVICE_TYPE_KEYBOARD,
		INPUT_DEVICE_TYPE_MOUSE,
		INPUT_DEVICE_TYPE_HID
	};

	/* A button/key's state */
	enum QUARTZ_API InputButtonState : UInt8
	{
		BUTTON_STATE_UP,
		BUTTON_STATE_DOWN,
	};

	/* Numeric id of a device */
	typedef UInt64 InputDeviceId;

	/* The desctiption/info of a device */
	struct QUARTZ_API InputDeviceInfo
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

	typedef void(*InputDeviceConnectCallbackFunc)(InputDeviceId deviceId, InputDeviceInfo deviceDesc);
	typedef void(*InputDeviceDisconnectCallbackFunc)(InputDeviceId deviceId);

	typedef void(*MouseMoveInputCallbackFunc)(InputDeviceId deviceId, Int64 relX, Int64 relY);
	typedef void(*MouseButtonInputCallbackFunc)(InputDeviceId deviceId, UInt32 button, InputButtonState state);
	typedef void(*MouseWheelInputCallbackFunc)(InputDeviceId deviceId, Float32 value);
	typedef void(*KeyboardInputCallbackFunc)(InputDeviceId deviceId, UInt32 scancode, InputButtonState state);
	typedef void(*ButtonInputCallbackFunc)(InputDeviceId deviceId, UInt32 button, InputButtonState state);
	typedef void(*AnalogInputCallbackFunc)(InputDeviceId deviceId, UInt32 analog, Float32 value);

	/* 
		Handles the connection of input devices and publishes input events
	*/
	class QUARTZ_API PlatformInput
	{
	protected:
		Set<InputDeviceId>					mConnectedDevices;
		Map<InputDeviceId, InputDeviceInfo> mDeviceInfos;

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

		/*
			Polls all device connections, populates the connected device infos list,
			and publish connection events accordingly.
		*/
		virtual void PollConnections() = 0;

		/*
			Polls all input from connected devices, and publishes events accordingly.
		*/
		virtual void PollInput() = 0;

		/*
			Restrict mouse cursor movement to bounds
		*/
		virtual void SetCursorBounds(Bounds2i bounds) = 0;

		/*
			Remove cursor movement bounds
		*/
		virtual void ReleaseCursorBounds() = 0;

		/*
			Disables the mouse cursor
		*/
		virtual void HideCursor() = 0;

		/*
			Enables the mouse cursor
		*/
		virtual void ShowCursor() = 0;

		/*
			Get the absolute position of the mouse
		*/
		virtual Point2i GetMousePosition() const = 0;

		/*
			Sets the absolute position of the mouse
		*/
		virtual void SetMousePosition(Point2i position) = 0;

		/*
			Check if device is connected
		*/
		Bool8 IsDeviceConnected(InputDeviceId deviceId);

		/*
			Get the device info of a device
		*/
		InputDeviceInfo* GetDeviceInfo(InputDeviceId deviceId);
	};
}