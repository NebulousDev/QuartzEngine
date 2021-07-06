#pragma once

#include "util/String.h"

namespace Quartz
{
	enum PeripheralType
	{
		PERIPHERAL_TYPE_KEYBOARD,
		PERIPHERAL_TYPE_MOUSE,
		PERIPHERAL_TYPE_CONTROLLER
	};

	struct QUARTZ_API Peripheral
	{
		StringW deviceName;
		StringW vendorName;
		StringW className;
		StringW pathName;
		StringW deviceId;
		StringW parentId;

		UInt32 usagePage;
		UInt32 usage;

		UInt32 buttonCount;
		UInt32 analogCount;

		PeripheralType type;

		Bool8 alive;
		Bool8 connected;
	};
}