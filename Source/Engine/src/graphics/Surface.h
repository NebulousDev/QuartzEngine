#pragma once

#include "Common.h"

namespace Quartz
{
	class QUARTZ_API Surface
	{
	protected:
		UInt32	mWidth;
		UInt32	mHeight;
		Bool8	mSupportsHDR;

		Surface(UInt32 width, UInt32 height, Bool8 supportsHDR);

	public:
		FORCE_INLINE UInt32 GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32 GetHeight() const { return mHeight; }
		FORCE_INLINE Bool8	SupportsHDR() const { return mSupportsHDR; }
	};
}