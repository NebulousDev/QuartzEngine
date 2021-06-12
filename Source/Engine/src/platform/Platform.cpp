
#include "VPPlatform.h"
#include "../log/Log.h"

namespace Quartz
{
	Platform* Platform::CreateInstanceImpl(Platform* pVirtualPlatform)
	{
		if (!GetInstance())
		{
			return pVirtualPlatform;
		}
		else
		{
			Log::Warning("Attempted to create a new platform instance when one already exists.");
		}

		return nullptr;
	}
}