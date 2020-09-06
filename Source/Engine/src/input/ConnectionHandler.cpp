#include "ConnectionHandler.h"

#include "../log/Log.h"
#include "../Engine.h"

#include "ConnectionEvent.h"

namespace Quartz
{
	void DeviceConnectCallback(InputDeviceId deviceId, InputDeviceInfo desc)
	{
#if 0
		Log.Info(L"Device Connected \n   | TYPE: %s\n   | NAME: %s\n   | ID:   %llu", 
			desc.deviceType == INPUT_DEVICE_TYPE_KEYBOARD ? L"KEYBOARD" : (desc.deviceType == INPUT_DEVICE_TYPE_MOUSE ? L"MOUSE" : L"HID"),
			desc.deviceName.Str(), 
			deviceId);
#endif

#if 1
		Log.Info(L"Device Connected [TYPE: %s | NAME: %s | ID: %llu]",
			desc.deviceType == INPUT_DEVICE_TYPE_KEYBOARD ? L"KEYBOARD" : (desc.deviceType == INPUT_DEVICE_TYPE_MOUSE ? L"MOUSE" : L"HID"),
			desc.deviceName.Str(),
			deviceId);
#endif

		ConnectionEvent connectionEvent;
		connectionEvent.desc = desc;

		Engine& engine = Engine::GetInstanceHandle();
		engine.GetEventSystem().Publish(EVENT_PRIORITY_MEDIUM, connectionEvent);
	}

	void DeviceDisconnectCallback(InputDeviceId deviceId)
	{
		InputDeviceInfo* pInfo;

		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();

		if ((pInfo = input.GetDeviceInfo(deviceId)) != nullptr)
		{
			Log.Info(L"Device Disconnected [TYPE: %s | NAME: %s | ID: %llu]",
				pInfo->deviceType == INPUT_DEVICE_TYPE_KEYBOARD ? L"KEYBOARD" : (pInfo->deviceType == INPUT_DEVICE_TYPE_MOUSE ? L"MOUSE" : L"HID"),
				pInfo->deviceName.Str(),
				deviceId);
		}
		else
		{
			// Error?
		}
	}

	void ConnectionHandler::Init()
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		input.SetDeviceConnectCallback(DeviceConnectCallback);
		input.SetDeviceDisconnectCallback(DeviceDisconnectCallback);
		input.PollConnections();
	}

	void ConnectionHandler::Update()
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		input.PollConnections();
	}

	void ConnectionHandler::Destroy()
	{

	}

	Bool8 ConnectionHandler::IsDeviceConnected(InputDeviceId deviceId)
	{
		Engine& engine = Engine::GetInstanceHandle();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		return input.IsDeviceConnected(deviceId);
	}
}

