#include "Win32Input.h"

//#include "../Win32.h"
#include "io/Log.h"
#include "util\Array.h"

//#include <hidsdi.h>
#pragma comment(lib,"Hid.lib")

#define RAW_INPUT_ERROR -1

namespace Quartz
{
	constexpr float IgnoreDeadzone(float x)
	{
		if ((x) > -0.26f && (x) < 0.26f)
			x = 0;
		if (x < -1.0f)
			x = -1.0f;
		if (x > 1.0f)
			x = 1.0f;
		return x;
	}

	void Win32Input::InitializeInput()
	{
		PollDeviceConnections();
	}

	void Win32Input::PollDeviceConnections()
	{
		Array<RAWINPUTDEVICELIST> rawInputDeviceList;
		UInt32 deviceCount = 0;

		mConnectedInputDevices.Clear();

		if (GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == RAW_INPUT_ERROR)
		{
			Log.Error("Failed to get raw input device list : GetRawInputDeviceList failed!");
			return;
		}

		if (deviceCount == 0)
		{
			return; // No devices connected
		}

		rawInputDeviceList.Resize(deviceCount);

		if (GetRawInputDeviceList(rawInputDeviceList.Data(), &deviceCount, sizeof(RAWINPUTDEVICELIST)) == RAW_INPUT_ERROR)
		{
			Log.Error("Failed to get raw input device list : GetRawInputDeviceList failed!");
			return;
		}
	
		for (UInt32 i = 0; i < rawInputDeviceList.Size(); i++)
		{
			// Get device info

			RAWINPUTDEVICELIST& rawInputDevice = rawInputDeviceList[i];
			RID_DEVICE_INFO ridDeviceInfo;
			UInt32 deviceInfoLegth = 0;

			InputDeviceDescription inputDeviceDesc;

			if (GetRawInputDeviceInfoA(rawInputDevice.hDevice, RIDI_DEVICEINFO, nullptr, &deviceInfoLegth) == RAW_INPUT_ERROR)
			{
				Log.Error("Failed to get raw input device info : GetRawInputDeviceInfoA failed!");
				continue;
			}

			if (deviceInfoLegth != sizeof(RID_DEVICE_INFO))
			{
				Log.Warning("GetRawInputDeviceInfoA returned a size that is not equal to sizeof(RID_DEVICE_INFO)");
			}

			if (GetRawInputDeviceInfoA(rawInputDevice.hDevice, RIDI_DEVICEINFO, &ridDeviceInfo, &deviceInfoLegth) == RAW_INPUT_ERROR)
			{
				Log.Error("Failed to get raw input device info : GetRawInputDeviceInfoA failed!");
				continue;
			}

			// Get device name

			Array<char> pathNameBuffer; // TODO: replace with StringBuffer
			UInt32 pathNameBufferSize = 0;

			if (GetRawInputDeviceInfoA(rawInputDevice.hDevice, RIDI_DEVICENAME, nullptr, &pathNameBufferSize) == RAW_INPUT_ERROR)
			{
				Log.Error("Failed to get raw input device name : GetRawInputDeviceInfoA failed!");
				continue;
			}

			pathNameBuffer.Resize(pathNameBufferSize);

			if (GetRawInputDeviceInfoA(rawInputDevice.hDevice, RIDI_DEVICENAME, pathNameBuffer.Data(), &pathNameBufferSize) == RAW_INPUT_ERROR)
			{
				Log.Error("Failed to get raw input device name : GetRawInputDeviceInfoA failed!");
				continue;
			}

			Array<char> nameBuffer(256); // TODO: replace with StringBuffer
			UInt32 nameBufferSize = 128;

			HANDLE hidHandle = CreateFile(pathNameBuffer.Data(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

			if (hidHandle)
			{
				if (HidD_GetProductString(hidHandle, nameBuffer.Data(), nameBufferSize) == TRUE)
				{
					// Convert UNICODE to ANSI (hacky)
					for (UInt32 i = 0; i < 128; i++)
					{
						nameBuffer[i] = nameBuffer[i * 2];

						if (nameBuffer[i] == '#')
						{
							nameBuffer[i] = '\\';
						}
					}
				}
				else
				{
					// Couldn't find a name
					memcpy(nameBuffer.Data(), "Unknown Device Name", 20);
				}

				CloseHandle(hidHandle);
			}

			// Create device descriptions

			if (ridDeviceInfo.dwType == RIM_TYPEMOUSE)
			{
				inputDeviceDesc.type = INPUT_DEVICE_TYPE_MOUSE;
				inputDeviceDesc.deviceName = nameBuffer.Data();
				inputDeviceDesc.pathName = pathNameBuffer.Data();
				inputDeviceDesc.deviceID = i;
				inputDeviceDesc.buttonCount = ridDeviceInfo.mouse.dwNumberOfButtons;
				inputDeviceDesc.analogCount = 0;
				inputDeviceDesc.usagePage = 1;
				inputDeviceDesc.usage = 2;
				inputDeviceDesc.pNext = (void*)rawInputDevice.hDevice;

				Log.Info("[%d]Mouse Found \"%s\" [%d] (Buttons=%d)", i, nameBuffer.Data(), inputDeviceDesc.pNext, ridDeviceInfo.mouse.dwNumberOfButtons);
				//Log.Info("    PATH: %s", pathNameBuffer.Data());
			}

			else if (ridDeviceInfo.dwType == RIM_TYPEKEYBOARD)
			{
				inputDeviceDesc.type = INPUT_DEVICE_TYPE_KEYBOARD;
				inputDeviceDesc.deviceName = nameBuffer.Data();
				inputDeviceDesc.pathName = pathNameBuffer.Data();
				inputDeviceDesc.deviceID = i;
				inputDeviceDesc.buttonCount = ridDeviceInfo.keyboard.dwNumberOfKeysTotal;
				inputDeviceDesc.analogCount = 0;
				inputDeviceDesc.usagePage = 1;
				inputDeviceDesc.usage = 6;
				inputDeviceDesc.pNext = (void*)rawInputDevice.hDevice;

				Log.Info("[%d]Keyboard Found \"%s\" [%d] (Keys=%d)", i, nameBuffer.Data(), inputDeviceDesc.pNext, ridDeviceInfo.keyboard.dwNumberOfKeysTotal);
				//Log.Info("    PATH: %s", pathNameBuffer.Data());
			}

			if (ridDeviceInfo.dwType == RIM_TYPEHID)
			{
				// Get preparsed data

				Array<UInt8> preparsedDataBuffer;
				UInt32 preparsedDataBufferSize = 0;

				if (GetRawInputDeviceInfo(rawInputDevice.hDevice, RIDI_PREPARSEDDATA, NULL, &preparsedDataBufferSize) == RAW_INPUT_ERROR)
				{
					Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
					continue;
				}

				preparsedDataBuffer.Resize(preparsedDataBufferSize);

				if (GetRawInputDeviceInfo(rawInputDevice.hDevice, RIDI_PREPARSEDDATA, preparsedDataBuffer.Data(), &preparsedDataBufferSize) == RAW_INPUT_ERROR)
				{
					Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
					continue;
				}

				PHIDP_PREPARSED_DATA pPreparsedData = (PHIDP_PREPARSED_DATA)preparsedDataBuffer.Data();

				if (!pPreparsedData)
				{
					// Not a usable input device
					continue;
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
						Log.Error("Failed to get device button capabilites: HidP_GetButtonCaps failed!");
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
						Log.Error("Failed to get device analog capabilites: HidP_GetValueCaps failed!");
						//continue;
					}

					pValueCaps = (PHIDP_VALUE_CAPS)valueCapsBuffer.Data();
					numberOfValues = pValueCaps->Range.UsageMax - pValueCaps->Range.UsageMin + 1;
				}
				
				inputDeviceDesc.type = INPUT_DEVICE_TYPE_OTHER;
				inputDeviceDesc.deviceName = nameBuffer.Data();
				inputDeviceDesc.pathName = pathNameBuffer.Data();
				inputDeviceDesc.deviceID = i; //ridDeviceInfo.hid.dwProductId;
				inputDeviceDesc.buttonCount = numberOfButtons;
				inputDeviceDesc.analogCount = numberOfValues;
				inputDeviceDesc.usagePage = ridDeviceInfo.hid.usUsagePage;
				inputDeviceDesc.usage = ridDeviceInfo.hid.usUsage;
				inputDeviceDesc.pNext = (void*)rawInputDevice.hDevice;

				Log.Info("[%d]HID found \"%s\" [%d]", i, nameBuffer.Data(), inputDeviceDesc.pNext);
				//Log.Info("    PATH: %s", pathNameBuffer.Data());
			}

			mConnectedInputDevices.PushBack(inputDeviceDesc);

			//InputDevice device;
			//CreateDevice(inputDeviceDesc, 0, &device);
		}
	}

	Bool8 Win32Input::CreateDevice(InputDeviceDescription deviceDesc, Flags32 flags, InputDevice* pDevice)
	{
		HANDLE deviceHandle = (HANDLE)deviceDesc.pNext;

		RAWINPUTDEVICE rawInputDevice {};
		rawInputDevice.usUsagePage = deviceDesc.usagePage;
		rawInputDevice.usUsage = deviceDesc.usage;
		rawInputDevice.dwFlags = 0;
		rawInputDevice.hwndTarget = 0;

		if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
		{
			Log.Critical("Failed to register input device : RegisterRawInputDevices failed!");
			return false;
		}

		InputDeviceData data;
		data.description = deviceDesc;
		data.buttonStates.Resize(deviceDesc.buttonCount);
		data.analogStates.Resize(deviceDesc.analogCount);
		data.isConnected = true;
		data.isDirty = false;

		Handle64 inputDevice = mInputDeviceRegistry.Size();
		mInputDeviceRegistry.PushBack(data);

		mHandleToDevice[(Handle64)deviceHandle] = inputDevice;

		*pDevice = inputDevice;

		return true;
	}

	void Win32Input::DestroyDevice(InputDevice device)
	{

	}

#define MAX_INPUT_BUFFER_SIZE 16

	void Win32Input::PollDeviceInputs()
	{
		RAWINPUT inputBuffer[MAX_INPUT_BUFFER_SIZE]{};
		UInt32 bufferSize = sizeof(RAWINPUT) * MAX_INPUT_BUFFER_SIZE;

		Int32 inputCount = GetRawInputBuffer(inputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));
		
		for (UInt32 i = 0; i < inputCount; i++)
		{
			const InputDevice* pInputDevice = mHandleToDevice.Get((Handle64)inputBuffer[i].header.hDevice);

			if (!pInputDevice)
			{
				// Device is not registered, ignore
				continue;
			}

			InputDeviceData& deviceData = mInputDeviceRegistry[*pInputDevice];
			RAWINPUT& inputData = inputBuffer[i];

			if (inputBuffer[i].header.dwType == RIM_TYPEMOUSE)
			{
				if (inputData.data.mouse.usFlags & MOUSE_MOVE_RELATIVE)
				{
					deviceData.mouseState.relativeX = inputData.data.mouse.lLastX;
					deviceData.mouseState.relativeY = inputData.data.mouse.lLastY;
				}
				if (inputData.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					deviceData.mouseState.absolueX = inputData.data.mouse.lLastX;
					deviceData.mouseState.absolueY = inputData.data.mouse.lLastY;
				}

				const UInt32 maxButtonDownBit = 0x1 << (deviceData.description.buttonCount * 2);
				for (UInt32 j = 0x1, idx = 0; j < maxButtonDownBit; j <<= 2, idx++)
				{
					if (inputData.data.mouse.usButtonFlags & j)
					{
						deviceData.buttonStates[idx].buttonValue = BUTTON_STATE_DOWN;
					}
				}

				const UInt32 maxButtonUpBit = 0x2 << (deviceData.description.buttonCount * 2);
				for (UInt32 j = 0x2, idx = 0; j < maxButtonUpBit; j <<= 2, idx++)
				{
					if (inputData.data.mouse.usButtonFlags & j)
					{
						deviceData.buttonStates[idx].buttonValue = BUTTON_STATE_UP;
					}
				}

				if (inputData.data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
				{
					deviceData.mouseState.mouseWheel = (Float32)(Int16)inputData.data.mouse.usButtonData / (Float32)WHEEL_DELTA;
				}
			}

			else if (inputData.header.dwType == RIM_TYPEKEYBOARD)
			{
				Log.Debug("[%d] Key: %d, dh=%d", deviceData.description.deviceID, inputBuffer[i].data.keyboard.MakeCode, inputBuffer[i].header.hDevice);
			}

			else if (inputData.header.dwType == RIM_TYPEHID)
			{
				// Get preparsed data

				Array<UInt8> preparsedDataBuffer;
				UInt32 preparsedDataBufferSize = 0;

				if (GetRawInputDeviceInfo(inputBuffer[i].header.hDevice, RIDI_PREPARSEDDATA, NULL, &preparsedDataBufferSize) == RAW_INPUT_ERROR)
				{
					Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
					continue;
				}

				preparsedDataBuffer.Resize(preparsedDataBufferSize);

				if (GetRawInputDeviceInfo(inputBuffer[i].header.hDevice, RIDI_PREPARSEDDATA, preparsedDataBuffer.Data(), &preparsedDataBufferSize) == RAW_INPUT_ERROR)
				{
					Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
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
						Log.Error("Failed to get device button capabilites: HidP_GetButtonCaps failed!");
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
						Log.Error("Failed to get device analog capabilites: HidP_GetValueCaps failed!");
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
				if((result = HidP_GetUsages(HidP_Input, pButtonCaps->UsagePage, 0, pUsage, &usageLength, pPreparsedData,
					(PCHAR)inputBuffer[i].data.hid.bRawData, inputBuffer[i].data.hid.dwSizeHid)) != HIDP_STATUS_SUCCESS)
				{
					if (result == HIDP_STATUS_USAGE_NOT_FOUND)
					{
						// Not a valid conroller
						continue;
					}
					
					Log.Error("Failed to get device usage: HidP_GetUsages failed!");
					continue;
				}

				for (ULONG j = 0; i < usageLength; i++)
				{
					Log.Debug("Button: %d", pUsage[j]);
				}

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
							if (newValue != 0.0f) Log.Debug("X-Axis L: %f", newValue);
							break;
						case 0x31:
							if (newValue != 0.0f) Log.Debug("Y-Axis L: %f", -newValue);
							break;
						case 0x32:
							if (newValue != 0.0f) Log.Debug("Trigger L: %f", newValue);
							break;
						case 0x33:
							if (newValue != 0.0f) Log.Debug("X-Axis R: %f", newValue);
							break;
						case 0x34:
							if (newValue != 0.0f) Log.Debug("Y-Axis R: %f", newValue);
							break;
						case 0x35:
							if (newValue != 0.0f) Log.Debug("Trigger R: %f", newValue);
							break;
					}
					*/
				}

				deviceData.isDirty = true;
			}
		}

		// Values
	}
}

