#include "Win32Input.h"

#include "log/Log.h"

#include <initguid.h>
#include <Cfgmgr32.h>
#include <Devpkey.h>

namespace Quartz
{

#define INPUT_MAX_RAWINPUT_BUFFER_SIZE 64
#define INPUT_RAW_INPUT_ERROR -1

#define INPUT_UNKNOWN_DEVICE_PATH_STRING StringW(L"Unknown Device Path")
#define INPUT_UNKNOWN_DEVICE_NAME_STRING StringW(L"Unknown Device Name")
#define INPUT_UNKNOWN_DEVICE_VENDOR_STRING StringW(L"Unknown Device Vendor")
#define INPUT_UNKNOWN_DEVICE_CLASS_STRING StringW(L"Unknown Device Class")
#define INPUT_UNKNOWN_DEVICE_SERIAL_STRING StringW(L"Unknown Device Serial Number")

	void EnumerateDevices(Array<RAWINPUTDEVICELIST>& rawInputDeviceList)
	{
		UInt32 deviceCount = 0;

		if (GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == INPUT_RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device list : GetRawInputDeviceList failed!");
			return;
		}

		if (deviceCount == 0)
		{
			// No devices connected
			return;
		}

		rawInputDeviceList.Resize(deviceCount);

		if (GetRawInputDeviceList(rawInputDeviceList.Data(), &deviceCount, sizeof(RAWINPUTDEVICELIST)) == INPUT_RAW_INPUT_ERROR)
		{
			//Log.Error("Failed to get raw input device list : GetRawInputDeviceList failed!");
			rawInputDeviceList.Clear();
			return;
		}
	}

	Bool8 GetDeviceInfo(const RAWINPUTDEVICELIST& rawInputDevice, Win32InputDeviceInfo& infoOut)
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

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, nullptr, (PUINT)&bufferSize) == INPUT_RAW_INPUT_ERROR)
		{
			return false;
		}

		pathName.Resize(bufferSize);
		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICENAME, pathName.Data(), (PUINT)&bufferSize) == INPUT_RAW_INPUT_ERROR)
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
		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICEINFO, nullptr, (PUINT)&bufferSize) == INPUT_RAW_INPUT_ERROR)
		{
			return false;
		}

		if (bufferSize != sizeof(RID_DEVICE_INFO))
		{
			return false;
		}

		if (GetRawInputDeviceInfoW(rawInputDevice.hDevice, RIDI_DEVICEINFO, &ridDeviceInfo, (PUINT)&bufferSize) == INPUT_RAW_INPUT_ERROR)
		{
			return false;
		}

		infoOut.id = deviceId;
		infoOut.name = deviceName;
		infoOut.vendor = vendorName;
		infoOut.type = className;
		infoOut.fullPath = pathName;
		infoOut.parentId = parentId;

		switch (rawInputDevice.dwType)
		{
			case RIM_TYPEMOUSE:
			{
				infoOut.usagePage = 0x01;
				infoOut.usage = 0x02;
				break;
			}

			case RIM_TYPEKEYBOARD:
			{
				infoOut.usagePage = 0x01;
				infoOut.usage = 0x06;
				break;
			}

			case RIM_TYPEHID:
			{
				infoOut.usagePage = ridDeviceInfo.hid.usUsagePage;
				infoOut.usage = ridDeviceInfo.hid.usUsage;
				break;
			}

		default:
			break;
		}
	}

	Bool8 RegisterInputUsage(UInt16 usagePage, UInt16 usage, DWORD flags)
	{
		RAWINPUTDEVICE rawInputDeviceInfo{};
		rawInputDeviceInfo.usUsagePage = usagePage;
		rawInputDeviceInfo.usUsage = usage;
		rawInputDeviceInfo.dwFlags = flags;
		rawInputDeviceInfo.hwndTarget = 0;

		if (RegisterRawInputDevices(&rawInputDeviceInfo, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
		{
			return false;
		}
	}

	void Win32Input::PreInitialize()
	{
		
	}

	void Win32Input::Initialize()
	{
		PollConnections();
	}

	void Win32Input::SetMouseMoveCallback(VPMouseMoveCallbackFunc callback)
	{
		mVPMouseMoveCallbackFunc = callback;
	}

	void Win32Input::SetMouseButtonCallback(VPMouseButtonCallbackFunc callback)
	{
		mVPMouseButtonCallbackFunc = callback;
	}

	void Win32Input::SetKeyboardKeyCallback(VPKeyboardKeyCallbackFunc callback)
	{
		mVPKeyboardKeyCallbackFunc = callback;
	}

	void Win32Input::PollConnections()
	{
		Array<RAWINPUTDEVICELIST> rawInputDevices;

		EnumerateDevices(rawInputDevices);

		// We only care about active device handles
		mDeviceIds.Clear();

		for (const RAWINPUTDEVICELIST& rawInputDevice : rawInputDevices)
		{
			Win32InputDeviceInfo deviceInfo;
			Bool8 result = GetDeviceInfo(rawInputDevice, deviceInfo);

			/*
			Log::Info(L"[DeviceFound] Name: %s, Vendor: %s, Class: %s, \n\tID: %s \n\tParentID: %s", 
				deviceInfo.name.Str(), deviceInfo.vendor.Str(), deviceInfo.type.Str(),
				deviceInfo.id.Str(), deviceInfo.parentId.Str());
			*/

			mDeviceIds.Put((Handle64)rawInputDevice.hDevice, deviceInfo.id);

			switch (rawInputDevice.dwType)
			{
				case RIM_TYPEMOUSE:
				{
					if (!mMice.Contains(deviceInfo.id))
					{
						Win32InputMouse* pMouse = new Win32InputMouse();
						pMouse->hDevice = rawInputDevice.hDevice;
						pMouse->info = deviceInfo;

						mMice.Put(deviceInfo.id, pMouse);

						RegisterInputUsage(deviceInfo.usagePage, deviceInfo.usage, 0);// RIDEV_NOLEGACY);

						Log::Info(L"[Device: Mouse] Name: %s, Vendor: %s", deviceInfo.name.Str(), deviceInfo.vendor.Str());
					}
					else
					{
						mMice[deviceInfo.id]->hDevice = rawInputDevice.hDevice;
					}

					break;
				}

				case RIM_TYPEKEYBOARD:
				{
					if (!mKeyboards.Contains(deviceInfo.id))
					{
						Win32InputKeyboard* pKeyboard = new Win32InputKeyboard();
						pKeyboard->hDevice = rawInputDevice.hDevice;
						pKeyboard->info = deviceInfo;

						mKeyboards.Put(deviceInfo.id, pKeyboard);

						RegisterInputUsage(deviceInfo.usagePage, deviceInfo.usage, 0);// RIDEV_NOLEGACY);

						Log::Info(L"[Device: Keyboard] Name: %s, Vendor: %s", deviceInfo.name.Str(), deviceInfo.vendor.Str());
					}
					else
					{
						mKeyboards[deviceInfo.id]->hDevice = rawInputDevice.hDevice;
					}

					break;
				}

				case RIM_TYPEHID:
				{
					if (!mControllers.Contains(deviceInfo.id))
					{
						Win32InputController* pController = new Win32InputController();
						pController->hDevice = rawInputDevice.hDevice;
						pController->info = deviceInfo;

						mControllers.Put(deviceInfo.id, pController);

						RegisterInputUsage(deviceInfo.usagePage, deviceInfo.usage, 0);

						Log::Info(L"[Device: Controller] Name: %s, Vendor: %s", deviceInfo.name.Str(), deviceInfo.vendor.Str());
					}
					else
					{
						mControllers[deviceInfo.id]->hDevice = rawInputDevice.hDevice;
					}

					break;
				}

			default:
				// Error
				break;
			}
		}
	}

	void Win32Input::PollInput()
	{
		RAWINPUT inputBuffer[INPUT_MAX_RAWINPUT_BUFFER_SIZE]{};
		UInt32 bufferSize = sizeof(RAWINPUT) * INPUT_MAX_RAWINPUT_BUFFER_SIZE;

		Int32 inputCount = GetRawInputBuffer(inputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

		for (UInt32 i = 0; i < inputCount; i++)
		{
			if (!mDeviceIds.Contains((Handle64)inputBuffer[i].header.hDevice))
			{
				// Idk where these phantom hDevices come from
				// @TODO: Figure out why
				continue;
			}

			const DeviceId& deviceId = mDeviceIds[(Handle64)inputBuffer[i].header.hDevice];

			switch (inputBuffer[i].header.dwType)
			{
				case RIM_TYPEMOUSE:
				{
					Win32InputMouse* pMouse = mMice[deviceId];

					if (inputBuffer[i].data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
					{
						mVPMouseMoveCallbackFunc(
							static_cast<HVPInputMouse>(pMouse),
							static_cast<Int64>(inputBuffer[i].data.mouse.lLastX),
							static_cast<Int64>(inputBuffer[i].data.mouse.lLastY));
					}

					if (inputBuffer[i].data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
					{
						//deviceData.mouseState.absolueX = inputData.data.mouse.lLastX;
						//deviceData.mouseState.absolueY = inputData.data.mouse.lLastY;
					}

					if (inputBuffer[i].data.mouse.usButtonFlags)
					{
						const UInt32 maxButtonDownBit = 0x1 << (5/*inputBuffer[i]->buttonCount*/ * 2);
						for (UInt32 j = 0x1, idx = 0; j < maxButtonDownBit; j <<= 2, idx++)
						{
							if (inputBuffer[i].data.mouse.usButtonFlags & j)
							{
								mVPMouseButtonCallbackFunc(
									static_cast<HVPInputMouse>(pMouse),
									static_cast<Int32>(idx), BUTTON_STATE_DOWN);
							}
						}

						const UInt32 maxButtonUpBit = 0x2 << (5/*inputBuffer[i]->buttonCount*/ * 2);
						for (UInt32 j = 0x2, idx = 0; j < maxButtonUpBit; j <<= 2, idx++)
						{
							if (inputBuffer[i].data.mouse.usButtonFlags & j)
							{
								mVPMouseButtonCallbackFunc(
									static_cast<HVPInputMouse>(pMouse),
									static_cast<Int32>(idx), BUTTON_STATE_UP);
							}
						}
					}

					break;
				}

				case RIM_TYPEKEYBOARD:
				{
					Win32InputKeyboard* pKeyboard = mKeyboards[deviceId];

					mVPKeyboardKeyCallbackFunc(
						static_cast<HVPInputKeyboard>(pKeyboard),
						static_cast<UInt32>(inputBuffer[i].data.keyboard.MakeCode),
						inputBuffer[i].data.keyboard.Flags& RI_KEY_BREAK ? BUTTON_STATE_UP : BUTTON_STATE_DOWN);

					break;
				}

				case RIM_TYPEHID:
				{
					Win32InputController* pController = mControllers[deviceId];

					//Log::Debug(L"[CONTROLLER][%s]", pController->info.name.Str());

					break;
				}

			default:
				break;
			}
		}
	}
}

