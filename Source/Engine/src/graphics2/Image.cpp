#include "Image.h"

namespace Quartz
{
	Image::Image(ImageType type, UInt32 width, UInt32 height, UInt32 depth, 
		UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages)
		: mType(type),
		mWidth(width),
		mHeight(height),
		mDepth(depth),
		mLayers(layers),
		mMips(mips),
		mFormat(format),
		mUsages(usages)
	{
		// Nothing
	}

	ImageView::ImageView(Image* pImage, ImageViewType type,
		UInt32 width, UInt32 height, UInt32 depth, 
		UInt32 layers, UInt32 layerStart, UInt32 mips, UInt32 mipStart, 
		ImageFormat format, ImageViewUsage usage)
		: mpImage(pImage),
		mType(type),
		mLayerStart(layerStart),
		mMipStart(mipStart),
		mFormat(format),
		mImageViewUsage(usage)
	{
		// Nothing
	}
}

