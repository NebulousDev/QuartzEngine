#include "Win32PlatformInput.h"

//#include "io///Log.h"
#include "Common.h"
#include "util\Array.h"
#include <cstdio>

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

#define UNKNOWN_DEVICE_NAME_STRING		StringW(L"Unknown Device Name")
#define UNKNOWN_DEVICE_PATH_STRING		StringW(L"Unknown Device Path")
#define UNKNOWN_DEVICE_SERIAL_STRING	StringW(L"Unknown Device Serial Number")

	void EnumerateDevices(Array<RAWINPUTDEVICELIST>& rawInputDeviceList)
	{
		UInt32 deviceCount = 0;

		if (GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device list : GetRawInputDeviceList failed!");
			rawInputDeviceList.Clear();
			return;
		}

		if (deviceCount == 0)
		{
			// No devices connected
			rawInputDeviceList.Clear();
			return;
		}

		rawInputDeviceList.Resize(deviceCount);

		if (GetRawInputDeviceList(rawInputDeviceList.Data(), &deviceCount, sizeof(RAWINPUTDEVICELIST)) == RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device list : GetRawInputDeviceList failed!");
			rawInputDeviceList.Clear();
			return;
		}
	}

	Bool8 GetDeviceInfo(const RAWINPUTDEVICELIST& rawInputDevice, RID_DEVICE_INFO* info)
	{
		UInt32 deviceInfoLegth = 0;

		if (GetRawInputDeviceInfoA(rawInputDevice.hDevice, RIDI_DEVICEINFO, nullptr, &deviceInfoLegth) == RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device info : GetRawInputDeviceInfoA failed!");
			return false;
		}

		if (deviceInfoLegth != sizeof(RID_DEVICE_INFO))
		{
			//Log.Warning("GetRawInputDeviceInfoA returned a size that is not equal to sizeof(RID_DEVICE_INFO)");
		}

		if (GetRawInputDeviceInfoA(rawInputDevice.hDevice, RIDI_DEVICEINFO, info, &deviceInfoLegth) == RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device info : GetRawInputDeviceInfoA failed!");
			return false;
		}
	}

	StringW GetDevicePathName(const RAWINPUTDEVICELIST& rawInputDevice)
	{
		Array<wchar_t> pathNameBuffer; // TODO: replace with StringBuffer
		UInt32 pathNameBufferSize = 0;

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, nullptr, &pathNameBufferSize) == RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device name : GetRawInputDeviceInfoA failed!");
			return UNKNOWN_DEVICE_NAME_STRING;
		}

		pathNameBuffer.Resize(pathNameBufferSize);

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, pathNameBuffer.Data(), &pathNameBufferSize) == RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device name : GetRawInputDeviceInfoA failed!");
			return UNKNOWN_DEVICE_NAME_STRING;
		}

		return StringW(pathNameBuffer.Data());
	}

	StringW GetDeviceName(HANDLE hidHandle)
	{
		constexpr UInt32 nameBufferSize = 256;
		Array<wchar_t> nameBuffer(nameBufferSize); // TODO: replace with StringBuffer

		if (hidHandle)
		{
			if (HidD_GetProductString(hidHandle, nameBuffer.Data(), nameBufferSize) == FALSE)
			{
				// Couldn't find a name
				return UNKNOWN_DEVICE_PATH_STRING;
			}
		}

		return StringW(nameBuffer.Data());
	}

	StringW GetDeviceSerialNumber(HANDLE hidHandle)
	{
		constexpr UInt32 serialBufferSize = 256;
		Array<wchar_t> serialBuffer(serialBufferSize); // TODO: replace with StringBuffer

		if (hidHandle)
		{
			if (HidD_GetSerialNumberString(hidHandle, serialBuffer.Data(), serialBufferSize) == FALSE)
			{
				// Couldn't find a name
				return UNKNOWN_DEVICE_SERIAL_STRING;
			}
		}

		return StringW(serialBuffer.Data());
	}

	HANDLE GetDeviceHandle(const StringW& devicePath)
	{
		return CreateFileW(devicePath.Str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	}

	Bool8 GenerateDeviceInfo(InputDeviceId deviceId, const StringW& devicePath, const RAWINPUTDEVICELIST& rawInputDevice, InputDeviceInfo* pInfo)
	{
		RID_DEVICE_INFO ridDeviceInfo;
		if (!GetDeviceInfo(rawInputDevice, &ridDeviceInfo))
		{
			// Failed to retrieve device info, skip device
			return false;
		}

		HANDLE hidHandle = GetDeviceHandle(devicePath);

		StringW deviceName = GetDeviceName(hidHandle);
		StringW deviceSerialNumber = GetDeviceSerialNumber(hidHandle);

		CloseHandle(hidHandle);

		pInfo->deviceId = deviceId;
		pInfo->deviceName = deviceName.Str();
		pInfo->devicePath = devicePath.Str();
		pInfo->serialNuber = deviceSerialNumber.Str();
		pInfo->pHandle = (void*)rawInputDevice.hDevice;

		// Create device descriptions

		if (ridDeviceInfo.dwType == RIM_TYPEMOUSE)
		{
			pInfo->deviceType = INPUT_DEVICE_TYPE_MOUSE;
			pInfo->buttonCount = ridDeviceInfo.mouse.dwNumberOfButtons;
			pInfo->analogCount = 0;
			pInfo->usagePage = 1;
			pInfo->usage = 2;
		}

		else if (ridDeviceInfo.dwType == RIM_TYPEKEYBOARD)
		{
			pInfo->deviceType = INPUT_DEVICE_TYPE_KEYBOARD;
			pInfo->buttonCount = ridDeviceInfo.keyboard.dwNumberOfKeysTotal;
			pInfo->analogCount = 0;
			pInfo->usagePage = 1;
			pInfo->usage = 6;
		}

		else if (ridDeviceInfo.dwType == RIM_TYPEHID)
		{
			// Get preparsed data

			Array<UInt8> preparsedDataBuffer;
			UInt32 preparsedDataBufferSize = 0;

			if (GetRawInputDeviceInfo(rawInputDevice.hDevice, RIDI_PREPARSEDDATA, NULL, &preparsedDataBufferSize) == RAW_INPUT_ERROR)
			{
				//Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
				return false;
			}

			preparsedDataBuffer.Resize(preparsedDataBufferSize);

			if (GetRawInputDeviceInfo(rawInputDevice.hDevice, RIDI_PREPARSEDDATA, preparsedDataBuffer.Data(), &preparsedDataBufferSize) == RAW_INPUT_ERROR)
			{
				//Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
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
					//Log.Error("Failed to get device button capabilites: HidP_GetButtonCaps failed!");
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
					//Log.Error("Failed to get device analog capabilites: HidP_GetValueCaps failed!");
					//continue;
				}

				pValueCaps = (PHIDP_VALUE_CAPS)valueCapsBuffer.Data();
				numberOfValues = pValueCaps->Range.UsageMax - pValueCaps->Range.UsageMin + 1;
			}

			pInfo->deviceType = INPUT_DEVICE_TYPE_MOUSE;
			pInfo->buttonCount = numberOfButtons;
			pInfo->analogCount = numberOfValues;
			pInfo->usagePage = ridDeviceInfo.hid.usUsagePage;
			pInfo->usage = ridDeviceInfo.hid.usUsage;
		}

		return true;
	}

	void Win32PlatformInput::PollConnections()
	{
		mPolledRawInputDevices.Clear();
		mPolledIds.Clear();

		EnumerateDevices(mPolledRawInputDevices);

		for (const RAWINPUTDEVICELIST& rawInputDevice : mPolledRawInputDevices)
		{
			StringW devicePath = GetDevicePathName(rawInputDevice);
			InputDeviceId deviceId = devicePath.Hash();

			// Skip connected devices
			if (!IsDeviceConnected(deviceId))
			{
				InputDeviceInfo newDeviceInfo;
				if (GenerateDeviceInfo(deviceId, devicePath, rawInputDevice, &newDeviceInfo))
				{
					mConnectedDevices.Add(deviceId);
					mDeviceInfos.Put(deviceId, newDeviceInfo);
					mHandleToDevice.Put(reinterpret_cast<Handle64>(rawInputDevice.hDevice), deviceId);

					RAWINPUTDEVICE rawInputDeviceInfo{};
					rawInputDeviceInfo.usUsagePage = newDeviceInfo.usagePage;
					rawInputDeviceInfo.usUsage = newDeviceInfo.usage;
					rawInputDeviceInfo.dwFlags = 0;
					rawInputDeviceInfo.hwndTarget = 0;

					if (RegisterRawInputDevices(&rawInputDeviceInfo, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
					{
						//Log.Critical("Failed to register input device : RegisterRawInputDevices failed!");
						//return false;
					}

					if (mDeviceConnectCallbackFunc != nullptr)
					{
						mDeviceConnectCallbackFunc(deviceId, newDeviceInfo);
					}
				}
			}

			mPolledIds.Add(deviceId);
		}

		for (InputDeviceId pastId : mConnectedDevices)
		{
			if (!mPolledIds.Contains(pastId))
			{
				if (mDeviceDisconnectCallbackFunc != nullptr)
				{
					mDeviceDisconnectCallbackFunc(pastId);
				}
			}
		}

		// Psudo double buffering swap
		Swap(mConnectedDevices, mPolledIds);
	}

#define MAX_INPUT_BUFFER_SIZE 64

	void Win32PlatformInput::PollInput()
	{
		RAWINPUT inputBuffer[MAX_INPUT_BUFFER_SIZE]{};
		UInt32 bufferSize = sizeof(RAWINPUT) * MAX_INPUT_BUFFER_SIZE;

		Int32 inputCount = GetRawInputBuffer(inputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

		for (UInt32 i = 0; i < inputCount; i++)
		{
			InputDeviceId* pInputDeviceId = mHandleToDevice.Get(reinterpret_cast<Handle64>(inputBuffer[i].header.hDevice));

			if (!pInputDeviceId)
			{
				// Device is not registered, ignore
				continue;
			}

			InputDeviceId deviceId = *pInputDeviceId;
			InputDeviceInfo* pInputDeviceDesc = mDeviceInfos.Get(deviceId);

			RAWINPUT& inputData = inputBuffer[i];

			if (inputBuffer[i].header.dwType == RIM_TYPEMOUSE)
			{
				if (inputData.data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				{
					if (mMouseMoveInputCallbackFunc)
					{
						mMouseMoveInputCallbackFunc(deviceId,
							static_cast<Int64>(inputData.data.mouse.lLastX),
							static_cast<Int64>(inputData.data.mouse.lLastY));
					}
				}

				if (inputData.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					//deviceData.mouseState.absolueX = inputData.data.mouse.lLastX;
					//deviceData.mouseState.absolueY = inputData.data.mouse.lLastY;
				}

				if (inputData.data.mouse.usButtonFlags)
				{
					const UInt32 maxButtonDownBit = 0x1 << (pInputDeviceDesc->buttonCount * 2);
					for (UInt32 j = 0x1, idx = 0; j < maxButtonDownBit; j <<= 2, idx++)
					{
						if (inputData.data.mouse.usButtonFlags & j)
						{
							if (mMouseButtonInputCallbackFunc)
							{
								mMouseButtonInputCallbackFunc(deviceId, idx, BUTTON_STATE_DOWN);
							}
						}
					}

					const UInt32 maxButtonUpBit = 0x2 << (pInputDeviceDesc->buttonCount * 2);
					for (UInt32 j = 0x2, idx = 0; j < maxButtonUpBit; j <<= 2, idx++)
					{
						if (inputData.data.mouse.usButtonFlags & j)
						{
							if (mMouseButtonInputCallbackFunc)
							{
								mMouseButtonInputCallbackFunc(deviceId, idx, BUTTON_STATE_UP);
							}
						}
					}
				}

				if (inputData.data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
				{
					if (mMouseWheelInputCallbackFunc)
					{
						mMouseWheelInputCallbackFunc(deviceId, (Float32)(Int16)inputData.data.mouse.usButtonData / (Float32)WHEEL_DELTA);
					}
				}
			}

			else if (inputData.header.dwType == RIM_TYPEKEYBOARD)
			{
				if (mKeyboardInputCallbackFunc)
				{
					InputButtonState state = inputData.data.keyboard.Flags & RI_KEY_BREAK ? BUTTON_STATE_UP : BUTTON_STATE_DOWN;
					mKeyboardInputCallbackFunc(deviceId, inputData.data.keyboard.MakeCode, state);
				}
			}

			else if (inputData.header.dwType == RIM_TYPEHID)
			{
				// Get preparsed data

				Array<UInt8> preparsedDataBuffer;
				UInt32 preparsedDataBufferSize = 0;

				if (GetRawInputDeviceInfo(inputBuffer[i].header.hDevice, RIDI_PREPARSEDDATA, NULL, &preparsedDataBufferSize) == RAW_INPUT_ERROR)
				{
					//Log.Error("Failed to get device preparsed data: GetRawInputDeviceInfo failed!");
					continue;
				}

				preparsedDataBuffer.Resize(preparsedDataBufferSize);

				if (GetRawInputDeviceInfo(inputBuffer[i].header.hDevice, RIDI_PREPARSEDDATA, preparsedDataBuffer.Data(), &preparsedDataBufferSize) == RAW_INPUT_ERROR)
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
				if((result = HidP_GetUsages(HidP_Input, pButtonCaps->UsagePage, 0, pUsage, &usageLength, pPreparsedData,
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
					if (mButtonInputCallbackFunc)
					{
						UInt32 buttonIdx = (pUsage[j] - pButtonCaps->Range.UsageMin);
						mButtonInputCallbackFunc(deviceId, buttonIdx, BUTTON_STATE_DOWN);
					}

					printf("Button: %d\n", pUsage[j]);
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

	Point2i Win32PlatformInput::GetMousePosition() const
	{
		POINT cursor;
		GetCursorPos(&cursor);
		return Point2i(cursor.x, cursor.y);
	}

	void Win32PlatformInput::SetMousePosition(Point2i position)
	{
		SetCursorPos(position.x, position.y);
	}

	void Win32PlatformInput::HideCursor()
	{
		::ShowCursor(0);
	}

	void Win32PlatformInput::ShowCursor()
	{
		::ShowCursor(1);
	}

	void Win32PlatformInput::SetCursorBounds(Bounds2i bounds)
	{
		RECT clipRect { bounds.start.x, bounds.start.y, bounds.end.x, bounds.end.y };
		ClipCursor(&clipRect);
	}

	void Win32PlatformInput::ReleaseCursorBounds()
	{
		ClipCursor(NULL);
	}
}

