#pragma once

#include "RenderPass.h"
#include "Image.h"
#include "util/Array.h"

namespace Quartz
{
	class QUARTZ_API Framebuffer
	{
	private:
		Array<Image*>		mImages;
		Array<ImageView*>	mImageViews;
		UInt32				mWidth;
		UInt32				mHeight;

	protected:
		Framebuffer(const Array<Image*>& images, const Array<ImageView*>& imageViews, UInt32 width, UInt32 height);

	public:
		FORCE_INLINE UInt32				GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32				GetHeight() const { return mHeight; }
		FORCE_INLINE Array<Image*>&		GetImages() { return mImages; }
		FORCE_INLINE Array<ImageView*>& GetImageViews() { return mImageViews; }
	};
}