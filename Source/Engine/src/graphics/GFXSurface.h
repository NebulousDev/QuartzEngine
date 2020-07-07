#pragma once

#include "Common.h"
#include "GFXResource.h"
#include "util\Array.h"
#include "GFXImage.h"

namespace Quartz
{
	class QUARTZ_API GFXSurface : public GFXResource
	{
	protected:
		UInt32 mWidth;
		UInt32 mHeight;
		UInt32 mBackBufferCount;
	
	public:
		GFXSurface() = default;

		UInt32 GetWidth() const { return mWidth; }
		UInt32 GetHeight() const { return mHeight; }
		UInt32 GetBackBufferCount() const { return mBackBufferCount; }

		virtual UInt32 AcquireNextImageIndex() = 0;
	};
}