#include "Win32PeripheralController.h"

#include "platform/PeripheralEvent.h"
#include "log/Log.h"

#include <initguid.h>
#include <Cfgmgr32.h>
#include <Devpkey.h>

#include "Engine.h"

#pragma comment(lib,"Hid.lib")

namespace Quartz
{
	#define MAX_INPUT_BUFFER_SIZE 64

	#define INPUT_UNKNOWN_DEVICE_PATH_STRING		StringW(L"Unknown Device Path")
	#define INPUT_UNKNOWN_DEVICE_NAME_STRING		StringW(L"Unknown Device Name")
	#define INPUT_UNKNOWN_DEVICE_VENDOR_STRING		StringW(L"Unknown Device Vendor")
	#define INPUT_UNKNOWN_DEVICE_CLASS_STRING		StringW(L"Unknown Device Class")
	#define INPUT_UNKNOWN_DEVICE_SERIAL_STRING		StringW(L"Unknown Device Serial Number")

	Bool8 EnumerateDevices(Array<RAWINPUTDEVICELIST>& rawInputDeviceList)
	{
		UInt32 deviceCount = 0;

		if (GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == -1)
		{
			Log::Error("Failed to enumerate raw input device list: GetRawInputDeviceList failed!");
			return false;
		}

		if (deviceCount == 0)
		{
			Log::Warning("EnumerateDevices() returned no valid devices.");
			return false;
		}

		rawInputDeviceList.Resize(deviceCount);

		if (GetRawInputDeviceList(rawInputDeviceList.Data(), &deviceCount, sizeof(RAWINPUTDEVICELIST)) == -1)
		{
			Log::Error("Failed to enumerate raw input device list: GetRawInputDeviceList failed!");
			rawInputDeviceList.Clear();
			return false;
		}

		return true;
	}

	Bool8 MakePeripheral(const RAWINPUTDEVICELIST& rawInputDevice, Win32Peripheral& peripheralOut)
	{
		StringW deviceName;
		StringW vendorName;
		StringW className;
		StringW pathName;
		StringW deviceId;
		StringW parentId;

		DEVINST devInst;
		DEVPROPTYPE propertyType;
		ULONG bufferSize = 0;

		RID_DEVICE_INFO ridDeviceInfo;

		// Device Path 

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, nullptr, (PUINT)&bufferSize) == -1)
		{
			return false;
		}

		pathName.Resize(bufferSize);
		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, pathName.Data(), (PUINT)&bufferSize) == -1)
		{
			return false;
		}

		// Device Instance ID

		bufferSize = 0;
		CONFIGRET cr = ::CM_Get_Device_Interface_PropertyW(pathName.Str(),
			&DEVPKEY_Device_InstanceId, &propertyType, nullptr, &bufferSize, 0);
		if (cr != CR_BUFFER_SMALL)
		{
			return false;
		}

		deviceId.Resize(bufferSize);
		cr = ::CM_Get_Device_Interface_PropertyW(pathName.Str(),
			&DEVPKEY_Device_InstanceId, &propertyType, (PBYTE)deviceId.Data(), &bufferSize, 0);
		if (cr != CR_SUCCESS)
		{
			return false;
		}

		// Dev Instance

		cr = ::CM_Locate_DevNodeW(&devInst, (DEVINSTID_W)deviceId.Str(), CM_LOCATE_DEVNODE_NORMAL);
		if (cr != CR_SUCCESS)
		{
			return false;
		}

		// Friendly Device Name

		bufferSize = 0;
		cr = ::CM_Get_DevNode_PropertyW(devInst, &DEVPKEY_NAME, &propertyType, nullptr, &bufferSize, 0);
		if (cr != CR_BUFFER_SMALL)
		{
			return false;
		}

		deviceName.Resize(bufferSize);
		cr = ::CM_Get_DevNode_PropertyW(devInst, &DEVPKEY_NAME,
			&propertyType, (PBYTE)deviceName.Data(), &bufferSize, 0);
		if (cr != CR_SUCCESS)
		{
			deviceName = INPUT_UNKNOWN_DEVICE_NAME_STRING;
		}

		// Friendly Vendor Name

		bufferSize = 0;
		cr = ::CM_Get_DevNode_PropertyW(devInst,
			&DEVPKEY_Device_Manufacturer, &propertyType, nullptr, &bufferSize, 0);
		if (cr != CR_BUFFER_SMALL)
		{
			return false;
		}

		vendorName.Resize(bufferSize);
		cr = ::CM_Get_DevNode_PropertyW(devInst, &DEVPKEY_Device_Manufacturer, &propertyType, (PBYTE)vendorName.Data(), &bufferSize, 0);
		if (cr != CR_SUCCESS)
		{
			vendorName = INPUT_UNKNOWN_DEVICE_VENDOR_STRING;
		}

		// Device Class

		bufferSize = 0;
		cr = ::CM_Get_DevNode_PropertyW(devInst,
			&DEVPKEY_Device_Class, &propertyType, nullptr, &bufferSize, 0);
		if (cr != CR_BUFFER_SMALL)
		{
			return false;
		}

		className.Resize(bufferSize);
		cr = ::CM_Get_DevNode_PropertyW(devInst, &DEVPKEY_Device_Class, &propertyType, (PBYTE)className.Data(), &bufferSize, 0);
		if (cr != CR_SUCCESS)
		{
			vendorName = INPUT_UNKNOWN_DEVICE_CLASS_STRING;
		}

		// Device Parent Path

		bufferSize = 0;
		cr = ::CM_Get_DevNode_PropertyW(devInst,
			&DEVPKEY_Device_Parent, &propertyType, nullptr, &bufferSize, 0);
		if (cr != CR_BUFFER_SMALL)
		{
			return false;
		}

		parentId.Resize(bufferSize);
		cr = ::CM_Get_DevNode_PropertyW(devInst, &DEVPKEY_Device_Parent, &propertyType, (PBYTE)parentId.Data(), &bufferSize, 0);
		if (cr != CR_SUCCESS)
		{
			parentId = INPUT_UNKNOWN_DEVICE_CLASS_STRING;
		}

		// Page Usages

		bufferSize = 0;
		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICEINFO, nullptr, (PUINT)&bufferSize) == -1)
		{
			return false;
		}

		if (bufferSize != sizeof(RID_DEVICE_INFO))
		{
			return false;
		}

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICEINFO, &ridDeviceInfo, (PUINT)&bufferSize) == -1)
		{
			return false;
		}

		peripheralOut.deviceHandle	= rawInputDevice.hDevice;

		peripheralOut.deviceId		= deviceId;
		peripheralOut.deviceName	= deviceName;
		peripheralOut.vendorName	= vendorName;
		peripheralOut.className		= className;
		peripheralOut.pathName		= pathName;
		peripheralOut.parentId		= parentId;

		switch (rawInputDevice.dwType)
		{
		case RIM_TYPEMOUSE:
		{
			peripheralOut.usagePage		= 0x01;
			peripheralOut.usage			= 0x02;
			peripheralOut.buttonCount	= ridDeviceInfo.mouse.dwNumberOfButtons;
			peripheralOut.analogCount	= 0;
			peripheralOut.type			= PERIPHERAL_TYPE_MOUSE;

			break;
		}

		case RIM_TYPEKEYBOARD:
		{
			peripheralOut.usagePage		= 0x01;
			peripheralOut.usage			= 0x06;
			peripheralOut.buttonCount	= ridDeviceInfo.keyboard.dwNumberOfKeysTotal;
			peripheralOut.analogCount	= 0;
			peripheralOut.type			= PERIPHERAL_TYPE_KEYBOARD;

			break;
		}

		case RIM_TYPEHID:
		{
			Array<UInt8>	preparsedDataBuffer;
			UInt32			preparsedDataBufferSize = 0;

			if (GetRawInputDeviceInfo(rawInputDevice.hDevice, RIDI_PREPARSEDDATA, NULL, &preparsedDataBufferSize) == -1)
			{
				Log::Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
				return false;
			}

			preparsedDataBuffer.Resize(preparsedDataBufferSize);

			if (GetRawInputDeviceInfo(rawInputDevice.hDevice, RIDI_PREPARSEDDATA, preparsedDataBuffer.Data(), &preparsedDataBufferSize) == -1)
			{
				Log::Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
				return false;
			}

			PHIDP_PREPARSED_DATA pPreparsedData = (PHIDP_PREPARSED_DATA)preparsedDataBuffer.Data();

			if (!pPreparsedData)
			{
				// Not a usable input device
				return false;
			}

			// Get Device capabilities

			HIDP_CAPS capabilities;
			NTSTATUS status = HidP_GetCaps(pPreparsedData, &capabilities);

			// Get Button capabilities

			UInt32 numberOfButtons = 0;

			Array<HIDP_BUTTON_CAPS> buttonCapsBuffer(capabilities.NumberInputButtonCaps);
			UInt16 capsBufferLength = capabilities.NumberInputButtonCaps;
			PHIDP_BUTTON_CAPS pButtonCaps = nullptr;

			if (capabilities.NumberInputButtonCaps)
			{
				if (HidP_GetButtonCaps(HidP_Input, buttonCapsBuffer.Data(), &capsBufferLength, pPreparsedData) != HIDP_STATUS_SUCCESS)
				{
					Log::Error("Failed to get device button capabilites: HidP_GetButtonCaps failed!");
					//continue;
				}

				pButtonCaps = (PHIDP_BUTTON_CAPS)buttonCapsBuffer.Data();
				numberOfButtons = pButtonCaps->Range.UsageMax - pButtonCaps->Range.UsageMin + 1;
			}

			// Get Analog capabilities

			UInt32 numberOfValues = 0;

			Array<HIDP_VALUE_CAPS> valueCapsBuffer(capabilities.NumberInputValueCaps);
			UInt16 valueBufferLength = capabilities.NumberInputValueCaps;
			PHIDP_VALUE_CAPS pValueCaps = nullptr;

			if (capabilities.NumberInputValueCaps)
			{
				if (HidP_GetValueCaps(HidP_Input, valueCapsBuffer.Data(), &valueBufferLength, pPreparsedData) != HIDP_STATUS_SUCCESS)
				{
					Log::Error("Failed to get device analog capabilites: HidP_GetValueCaps failed!");
					//continue;
				}

				pValueCaps = (PHIDP_VALUE_CAPS)valueCapsBuffer.Data();
				numberOfValues = pValueCaps->Range.UsageMax - pValueCaps->Range.UsageMin + 1;
			}

			peripheralOut.usagePage		= ridDeviceInfo.hid.usUsagePage;
			peripheralOut.usage			= ridDeviceInfo.hid.usUsage;
			peripheralOut.buttonCount	= numberOfButtons;
			peripheralOut.analogCount	= numberOfValues;
			peripheralOut.type			= PERIPHERAL_TYPE_CONTROLLER;

			peripheralOut.buttonCapsBuffer	= buttonCapsBuffer;
			peripheralOut.pButtonCaps		= pButtonCaps;
			peripheralOut.valueCapsBuffer	= valueCapsBuffer;
			peripheralOut.pValueCaps		= pValueCaps;

			break;
		}

		default:
			break;
		}

		peripheralOut.alive = true;
		peripheralOut.connected = true;

		return true;
	}

	Bool8 Win32PeripheralController::RegisterInputUsage(UInt16 usagePage, UInt16 usage, DWORD flags)
	{
		RAWINPUTDEVICE rawInputDeviceInfo = {};
		rawInputDeviceInfo.usUsagePage	= usagePage;
		rawInputDeviceInfo.usUsage		= usage;
		rawInputDeviceInfo.dwFlags		= flags;
		rawInputDeviceInfo.hwndTarget	= 0;

		if (RegisterRawInputDevices(&rawInputDeviceInfo, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
		{
			return false;
		}
	}

	Bool8 IsValidPeripheral(const RAWINPUTDEVICELIST& rawInputDevice)
	{
		UINT bufferSize = 0;
		StringW pathName;

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, nullptr, &bufferSize) == -1)
		{
			return false;
		}

		pathName.Resize(bufferSize);
		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, pathName.Data(), &bufferSize) == -1)
		{
			return false;
		}

		HANDLE fileHandle = CreateFileW(pathName.Str(), 
			0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			// Skip invalid peripherals
			return false;
		}

		// TODO: Check for root?

		return true;
	}

	void Win32PeripheralController::RescanPeripherals()
	{
		Array<RAWINPUTDEVICELIST> rawInputDevices;
		EnumerateDevices(rawInputDevices);

		for (auto& pair : mPeripheralMap)
		{
			pair.value->alive = false;
		}

		for (const RAWINPUTDEVICELIST& rawInputDevice : rawInputDevices)
		{
			Peripheral** ppExisting = mPeripheralMap.Get((DeviceHandle)rawInputDevice.hDevice);

			if (ppExisting)
			{
				(*ppExisting)->alive = true;

				if (!(*ppExisting)->connected)
				{
					PeripheralEvent connectionEvent;
					connectionEvent.pPeripheral = (*ppExisting);
					connectionEvent.eventType	= PERIPHERAL_CONNECTED;
					Engine::GetInstance()->GetEventSystem()->Publish(connectionEvent);
				}

				(*ppExisting)->connected = true;
			}
			else
			{
				if (IsValidPeripheral(rawInputDevice))
				{
					Win32Peripheral* pPeripheral = new Win32Peripheral();
					MakePeripheral(rawInputDevice, *pPeripheral);
					mPeripheralMap.Put((DeviceHandle)rawInputDevice.hDevice, pPeripheral);

					PeripheralEvent connectionEvent;
					connectionEvent.pPeripheral = pPeripheral;
					connectionEvent.eventType = PERIPHERAL_CONNECTED;
					Engine::GetInstance()->GetEventSystem()->Publish(connectionEvent);

					RegisterInputUsage(pPeripheral->usagePage, pPeripheral->usage, 0);
				}
			}
		}

		for (auto& pair : mPeripheralMap)
		{
			if (pair.value->alive == false)
			{
				if (pair.value->connected)
				{
					pair.value->connected = false;

					PeripheralEvent connectionEvent;
					connectionEvent.pPeripheral = pair.value;
					connectionEvent.eventType	= PERIPHERAL_DISCONNECTED;
					Engine::GetInstance()->GetEventSystem()->Publish(connectionEvent);
				}
			}
		}
	}

	void Win32PeripheralController::PollInput()
	{
		RAWINPUT inputBuffer[MAX_INPUT_BUFFER_SIZE]{};
		UInt32 bufferSize = sizeof(RAWINPUT) * MAX_INPUT_BUFFER_SIZE;

		Int32 inputCount = GetRawInputBuffer(inputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

		for (UInt32 i = 0; i < inputCount; i++)
		{
			Peripheral** ppPeripheral = mPeripheralMap.Get(reinterpret_cast<DeviceHandle>(inputBuffer[i].header.hDevice));

			if (!ppPeripheral)
			{
				// Device is not registered, ignore
				continue;
			}

			Peripheral* pPeripheral = *ppPeripheral;

			RAWINPUT& inputData = inputBuffer[i];

			if (inputBuffer[i].header.dwType == RIM_TYPEMOUSE)
			{
				if (inputData.data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				{
					Engine::GetInstance()->GetInputSystem()->TriggerMouseMoveInputAction((Mouse*)pPeripheral, 
						{ (Float32)inputData.data.mouse.lLastX , (Float32)inputData.data.mouse.lLastY });
				}

				if (inputData.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					// TODO
				}

				if (inputData.data.mouse.usButtonFlags)
				{
					const UInt32 maxButtonDownBit = 0x1 << (pPeripheral->buttonCount * 2);
					for (UInt32 j = 0x1, idx = 0; j < maxButtonDownBit; j <<= 2, idx++)
					{
						if (inputData.data.mouse.usButtonFlags & j)
						{
							Engine::GetInstance()->GetInputSystem()->TriggerMouseButtonInputAction(
								(Mouse*)pPeripheral, idx, BUTTON_ACTION_DOWN);
						}
					}

					const UInt32 maxButtonUpBit = 0x2 << (pPeripheral->buttonCount * 2);
					for (UInt32 j = 0x2, idx = 0; j < maxButtonUpBit; j <<= 2, idx++)
					{
						if (inputData.data.mouse.usButtonFlags & j)
						{
							Engine::GetInstance()->GetInputSystem()->TriggerMouseButtonInputAction(
								(Mouse*)pPeripheral, idx, BUTTON_ACTION_UP);
						}
					}
				}

				if (inputData.data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
				{
					Float32 value = (Float32)(Int16)inputData.data.mouse.usButtonData / (Float32)WHEEL_DELTA;
					Engine::GetInstance()->GetInputSystem()->TriggerMouseScrollInputAction((Mouse*)pPeripheral, 
						value, value > 0 ? MOUSE_WHEEL_ACTION_UP : MOUSE_WHEEL_ACTION_DOWN);
				}
			}

			else if (inputData.header.dwType == RIM_TYPEKEYBOARD)
			{
				ButtonActions actions = inputData.data.keyboard.Flags & RI_KEY_BREAK ? BUTTON_ACTION_UP : BUTTON_ACTION_DOWN;
				Engine::GetInstance()->GetInputSystem()->TriggerKeyboardInputAction((Keyboard*)pPeripheral,
					inputData.data.keyboard.MakeCode, actions);
			}

			else if (inputData.header.dwType == RIM_TYPEHID)
			{
				// Get preparsed data

				Array<UInt8> preparsedDataBuffer;
				UInt32 preparsedDataBufferSize = 0;

				if (GetRawInputDeviceInfo(inputBuffer[i].header.hDevice, RIDI_PREPARSEDDATA, NULL, &preparsedDataBufferSize) == -1)
				{
					//Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
					continue;
				}

				preparsedDataBuffer.Resize(preparsedDataBufferSize);

				if (GetRawInputDeviceInfo(inputBuffer[i].header.hDevice, RIDI_PREPARSEDDATA, preparsedDataBuffer.Data(), &preparsedDataBufferSize) == -1)
				{
					//Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
					continue;
				}

				PHIDP_PREPARSED_DATA pPreparsedData = (PHIDP_PREPARSED_DATA)preparsedDataBuffer.Data();

				// Get Device capabilities

				HIDP_CAPS capabilities;
				NTSTATUS status = HidP_GetCaps(pPreparsedData, &capabilities);

				// Get Button capabilities

				UInt32 numberOfButtons = 0;

				Array<HIDP_BUTTON_CAPS> buttonCapsBuffer(capabilities.NumberInputButtonCaps);
				UInt16 capsBufferLength = capabilities.NumberInputButtonCaps;
				PHIDP_BUTTON_CAPS pButtonCaps = nullptr;

				if (capabilities.NumberInputButtonCaps)
				{
					if (HidP_GetButtonCaps(HidP_Input, buttonCapsBuffer.Data(), &capsBufferLength, pPreparsedData) != HIDP_STATUS_SUCCESS)
					{
						//Log.Error("Failed to get device button capabilites: HidP_GetButtonCaps failed!");
						continue;
					}

					pButtonCaps = (PHIDP_BUTTON_CAPS)buttonCapsBuffer.Data();
					numberOfButtons = pButtonCaps->Range.UsageMax - pButtonCaps->Range.UsageMin + 1;
				}

				// Get Analog capabilities

				UInt32 numberOfValues = 0;

				Array<HIDP_VALUE_CAPS> valueCapsBuffer(capabilities.NumberInputValueCaps);
				UInt16 valueBufferLength = capabilities.NumberInputValueCaps;
				PHIDP_VALUE_CAPS pValueCaps = nullptr;

				if (capabilities.NumberInputValueCaps)
				{
					if (HidP_GetValueCaps(HidP_Input, valueCapsBuffer.Data(), &valueBufferLength, pPreparsedData) != HIDP_STATUS_SUCCESS)
					{
						//Log.Error("Failed to get device analog capabilites: HidP_GetValueCaps failed!");
						continue;
					}

					pValueCaps = (PHIDP_VALUE_CAPS)valueCapsBuffer.Data();
					numberOfValues = capabilities.NumberInputValueCaps;
				}

				// Usages

				Array<USAGE> usageBuffer(numberOfButtons);

				PUSAGE pUsage = (PUSAGE)usageBuffer.Data();
				ULONG usageLength = numberOfButtons;

				Int32 result;
				if ((result = HidP_GetUsages(HidP_Input, pButtonCaps->UsagePage, 0, pUsage, &usageLength, pPreparsedData,
					(PCHAR)inputBuffer[i].data.hid.bRawData, inputBuffer[i].data.hid.dwSizeHid)) != HIDP_STATUS_SUCCESS)
				{
					if (result == HIDP_STATUS_USAGE_NOT_FOUND)
					{
						// Not a valid conroller
						continue;
					}

					//Log.Error("Failed to get device usage: HidP_GetUsages failed!");
					continue;
				}

				for (ULONG j = 0; j < usageLength; j++)
				{
					/*
					if (mButtonInputCallbackFunc)
					{
						UInt32 buttonIdx = (pUsage[j] - pButtonCaps->Range.UsageMin);
						mButtonInputCallbackFunc(deviceId, buttonIdx, BUTTON_STATE_DOWN);
					}

					printf("Button: %d\n", pUsage[j]);
					*/
				}

				/*
				for (UInt32 j = 0; j < numberOfValues; j++)
				{
					ULONG value;
					result = HidP_GetUsageValue(HidP_Input, pValueCaps[j].UsagePage, 0, pValueCaps[j].Range.UsageMin,
						&value, pPreparsedData, (PCHAR)inputBuffer[i].data.hid.bRawData, inputBuffer[i].data.hid.dwSizeHid);

					float adjustedValue = (((Float32)value / 65535.0f) * 2) - 1.0f;

					for (UInt32 k = pValueCaps[j].Range.UsageMin; k < pValueCaps[j].Range.UsageMax; k++)
					{
						deviceData.analogStates[k].value = adjustedValue;
					}

					/*
					switch (pValueCaps[j].Range.UsageMin)
					{
						case 0x30:
							if (newValue != 0.0f) //Log.Debug("X-Axis L: %f", newValue);
							break;
						case 0x31:
							if (newValue != 0.0f) //Log.Debug("Y-Axis L: %f", -newValue);
							break;
						case 0x32:
							if (newValue != 0.0f) //Log.Debug("Trigger L: %f", newValue);
							break;
						case 0x33:
							if (newValue != 0.0f) //Log.Debug("X-Axis R: %f", newValue);
							break;
						case 0x34:
							if (newValue != 0.0f) //Log.Debug("Y-Axis R: %f", newValue);
							break;
						case 0x35:
							if (newValue != 0.0f) //Log.Debug("Trigger R: %f", newValue);
							break;
					}

				}
				*/
			}
		}

		// Values
	}

	Bool8 Win32PeripheralController::IsConnected(Peripheral* pPeripheral)
	{
		Win32Peripheral* pWin32Peripheral = static_cast<Win32Peripheral*>(pPeripheral);
		Peripheral** ppPeripheral = mPeripheralMap.Get((DeviceHandle)pWin32Peripheral->deviceHandle);

		if (ppPeripheral)
		{
			return (*ppPeripheral)->connected;
		}

		return false;
	}
}

