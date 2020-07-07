#include "GFXContext.h"

namespace Quartz
{
	void GFXContext::SetDebugName(GFXResource* pResource, const String& debugName)
	{
		pResource->SetDebugName(debugName);
		SetDebugNameImpl(pResource, debugName);
	}
}

