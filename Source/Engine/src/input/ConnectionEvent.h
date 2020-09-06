#pragma once

#include "util\String.h"
#include "../event/Event.h"
#include "PlatformInput.h"

namespace Quartz
{
	struct ConnectionEvent : public Event<ConnectionEvent>
	{
		InputDeviceInfo desc;
	};
}