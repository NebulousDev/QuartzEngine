#include "DeviceConnectionSystem.h"

#include "../log/Log.h"
#include "../Engine.h"

#include "DeviceConnectionEvent.h"

namespace Quartz
{
	void DeviceConnectCallback(InputDeviceId deviceId, InputDeviceDesc desc)
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

		DeviceConnectionEvent connectionEvent;
		connectionEvent.desc = desc;

		Engine& engine = Engine::GetInstance();
		engine.GetEventSystem().Publish(EVENT_PRIORITY_MEDIUM, connectionEvent);
	}

	void DeviceDisconnectCallback(InputDeviceId deviceId)
	{
		InputDeviceDesc desc;

		Engine& engine = Engine::GetInstance();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();

		if (input.GetDeviceDescription(deviceId, &desc))
		{
			Log.Info(L"Device Disconnected [TYPE: %s | NAME: %s | ID: %llu]",
				desc.deviceType == INPUT_DEVICE_TYPE_KEYBOARD ? L"KEYBOARD" : (desc.deviceType == INPUT_DEVICE_TYPE_MOUSE ? L"MOUSE" : L"HID"),
				desc.deviceName.Str(),
				deviceId);
		}
		else
		{
			// Error?
		}
	}

	void DeviceConnectionSystem::Initialize()
	{
		Engine& engine = Engine::GetInstance();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		input.SetDeviceConnectCallback(DeviceConnectCallback);
		input.SetDeviceDisconnectCallback(DeviceDisconnectCallback);
		input.PollDeviceConnections();
	}

	void DeviceConnectionSystem::Update()
	{
		
	}

	void DeviceConnectionSystem::Tick()
	{
		Engine& engine = Engine::GetInstance();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		input.PollDeviceConnections();
	}

	void DeviceConnectionSystem::Destroy()
	{

	}

	Bool8 DeviceConnectionSystem::IsDeviceConnected(InputDeviceId deviceId)
	{
		Engine& engine = Engine::GetInstance();
		PlatformInput& input = engine.GetPlatform().GetPlatformInput();
		return input.IsDeviceConnected(deviceId);
	}
}

