#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\Map.h"
#include "util\String.h"

namespace Quartz
{
	enum QUARTZ_API InputDeviceType
	{
		INPUT_DEVICE_TYPE_KEYBOARD,
		INPUT_DEVICE_TYPE_MOUSE,
		INPUT_DEVICE_TYPE_OTHER
	};

	enum QUARTZ_API ButtonState : UInt8
	{
		BUTTON_STATE_UP,
		BUTTON_STATE_DOWN,
	};

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

	struct QUARTZ_API InputDeviceDescription
	{
		InputDeviceType type;
		String			deviceName;
		String			pathName;
		UInt32			deviceID;
		UInt16			buttonCount;
		UInt16			analogCount;
		UInt16			usagePage;
		UInt16			usage;
		void*			pNext;
	};

	struct QUARTZ_API InputDeviceData
	{
		InputDeviceDescription	description;
		InputMouseState			mouseState;
		Array<InputButtonState> buttonStates;
		Array<InputAnalogState> analogStates;
		Bool8					isConnected;
		Bool8					isDirty;
	};

	typedef Handle32 InputDevice;

	class QUARTZ_API Input
	{
	protected:
		Array<InputDeviceDescription> mConnectedInputDevices;
		Array<InputDeviceData> mInputDeviceRegistry;

		Bool8 mInitialized;

	public:
		virtual void InitializeInput() = 0;
		virtual void PollDeviceConnections() = 0;
		virtual Bool8 CreateDevice(InputDeviceDescription deviceDesc, Flags32 flags, InputDevice* pDevice) = 0;
		virtual void DestroyDevice(InputDevice device) = 0;
		virtual void PollDeviceInputs() = 0;

		void Update();

		const InputButtonState& GetButtonState(InputDevice device, UInt32 buttonID) const;
		const InputAnalogState& GetAnalogState(InputDevice device, UInt32 analogID) const;

		FORCE_INLINE const Array<InputDeviceDescription>& GetConnectedDeviceList() const; 
	};
}