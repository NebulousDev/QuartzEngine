#include "GFXUtil.h"

namespace Quartz
{
	String DeviceVendorNameById(UInt32 vendorID)
	{
		switch (vendorID)
		{
			case 0x1002: return STRING("AMD");
			case 0x1010: return STRING("ImgTec");
			case 0x10DE: return STRING("NVIDIA");
			case 0x13B5: return STRING("ARM");
			case 0x5143: return STRING("Qualcomm");
			case 0x8086: return STRING("INTEL");
			default:	 return STRING("Unknown");
		}
	}
}

