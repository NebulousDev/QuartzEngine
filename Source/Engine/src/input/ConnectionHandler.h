#pragma once

#include "../entity/System.h"
#include "PlatformInput.h"

namespace Quartz
{
	class QUARTZ_API ConnectionHandler
	{
	private:
		friend void DeviceConnectCallback(InputDeviceId deviceId, InputDeviceInfo desc);
		friend void DeviceDisconnectCallback(InputDeviceId deviceId);

	public:
		void Init();
		void Update();
		void Destroy();

		Bool8 IsDeviceConnected(InputDeviceId deviceId);
	};
}