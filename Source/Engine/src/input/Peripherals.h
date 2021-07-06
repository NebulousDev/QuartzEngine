#pragma once

#include "util/String.h"
#include "../platform/Peripheral.h"

namespace Quartz
{
	class QUARTZ_API Mouse : public Peripheral
	{
		UInt32 numButtons;
	};

	class QUARTZ_API Keyboard : public Peripheral
	{
		UInt32 numKeys;
	};
}