#pragma once

#include "../system/System.h"
#include "PlatformInput.h"

namespace Quartz
{
	class QUARTZ_API DeviceConnectionSystem : public System
	{
	private:
		friend void DeviceConnectCallback(InputDeviceId deviceId, InputDeviceDesc desc);
		friend void DeviceDisconnectCallback(InputDeviceId deviceId);

	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;

		Bool8 IsDeviceConnected(InputDeviceId deviceId);
	};
}