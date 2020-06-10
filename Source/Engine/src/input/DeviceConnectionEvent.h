#pragma once

#include "util\String.h"
#include "../event/Event.h"
#include "PlatformInput.h"

namespace Quartz
{
	struct DeviceConnectionEvent : public Event<DeviceConnectionEvent>
	{
		InputDeviceDesc desc;
	};
}