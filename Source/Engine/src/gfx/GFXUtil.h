#pragma once
#include "Common.h"

namespace Quartz
{
	const Char* GFXDeviceVendorIDName(UInt32 vendorID)
	{
		switch (vendorID)
		{
		case 0x1002: return "AMD";
		case 0x1010: return "ImgTec";
		case 0x10DE: return "NVIDIA";
		case 0x13B5: return "ARM";
		case 0x5143: return "Qualcomm";
		case 0x8086: return "INTEL";
		default: return NULL;
		}
	}
}