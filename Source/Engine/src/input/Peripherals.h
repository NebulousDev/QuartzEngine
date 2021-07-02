#pragma once

#include "util/String.h"

namespace Quartz
{
	class QUARTZ_API Peripheral
	{
		String name;
	};

	class QUARTZ_API Mouse : public Peripheral
	{
		UInt32 numButtons;
	};

	class QUARTZ_API Keyboard : public Peripheral
	{
		UInt32 numKeys;
	};
}