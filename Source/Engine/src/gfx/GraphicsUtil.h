#pragma once

#include "PhysicalDevice.h"
#include "util\Array.h"
#include "util\String.h"

namespace Quartz
{
	QUARTZ_API String DeviceVendorNameById(UInt32 vendorID);

	QUARTZ_API PhysicalDevice* FindPrimaryPhysicalDevice(Array<PhysicalDevice*>& physcialDevices);
}