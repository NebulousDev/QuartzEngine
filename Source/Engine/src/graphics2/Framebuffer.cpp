#include "Framebuffer.h"

namespace Quartz
{
	Framebuffer::Framebuffer(const Array<Image*>& images, const Array<ImageView*>& imageViews, UInt32 width, UInt32 height)
		: mImages(images),
		mImageViews(imageViews),
		mWidth(width),
		mHeight(height)
	{
		// Nothing
	}
}

