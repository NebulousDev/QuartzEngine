#pragma once

#include "Common.h"

#include "GFXResource.h"
#include "GFXFormat.h"

namespace Quartz
{
	enum GFXImageType
	{
		IMAGE_TYPE_1D,
		IMAGE_TYPE_2D,
		IMAGE_TYPE_3D
	};

	enum GFXImageUsage
	{
		IMAGE_USAGE_SAMPLED_TEXTURE,
		IMAGE_USAGE_COLOR_ATTACHMENT,
		IMAGE_USAGE_DEPTH_ATTACHMENT,
		IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT
	};

	class QUARTZ_API GFXImage : public GFXResource
	{
	protected:
		UInt32 mWidth;
		UInt32 mHeight;
		UInt32 mDepth;
		UInt32 mMipLevels;
		UInt32 mLayers;

		GFXFormat mImageFormat;
		GFXImageType mImageType;

		GFXImage() = default;

	public:
		UInt32 GetWidth() const { return mWidth; }
		UInt32 GetHeight() const { return mWidth; }
		UInt32 GetDepth() const { return mWidth; }
		UInt32 GetMipLevels() const { return mMipLevels; }
		UInt32 GetLayers() const { return mLayers; }
		GFXFormat GetFormat() const { return mImageFormat; }
		GFXImageType GetType() const { return mImageType; }
	};

	class QUARTZ_API GFXImageView : public GFXResource
	{
	protected:
		GFXImage* mpImage;
		UInt32 mMipStart;
		UInt32 mMipCount;
		UInt32 mLayerStart;
		UInt32 mLayerCount;

		GFXImageView() = default;

	public:
		GFXImage& GetImage() { return *mpImage; }

		UInt32 GetMipStart() const { return mMipStart; }
		UInt32 GetMipCount() const { return mMipCount; }
		UInt32 GetLayerStart() const { return mLayerStart; }
		UInt32 GetLayerCount() const { return mLayerCount; }
	};
}