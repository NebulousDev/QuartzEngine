#pragma once

#include "Common.h"

#include "util\Castable.h"
#include "util\String.h"

namespace Quartz
{
	class QUARTZ_API GFXResource : public Castable
	{
	public:
		friend class GFXContext;

	private:
#if QUARTZ_USE_DEBUG_NAMES
		String mDebugName;
#endif
		void SetDebugName(const String& name)
#if QUARTZ_USE_DEBUG_NAMES
		{ mDebugName = name; }
#else
		{}
#endif

	public:
		const String& GetDebugName() const 
#if QUARTZ_USE_DEBUG_NAMES
		{ return mDebugName; }
#else
		{ return String("Unnamed Resource"); }
#endif

	};
}