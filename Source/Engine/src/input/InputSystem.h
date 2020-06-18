#pragma once

#include "../system/System.h"
#include "PlatformInput.h"
#include "util\RefPtr.h"

namespace Quartz
{
	class QUARTZ_API InputSystem : public System
	{
	private:
		friend void MouseMoveInputCallback(InputDeviceId deviceId, Int64 relX, Int64 relY);
		friend void MouseButtonInputCallback(InputDeviceId deviceId, UInt32 button, ButtonState state);
		friend void MouseWheelInputCallback(InputDeviceId deviceId, Float32 value);

	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}