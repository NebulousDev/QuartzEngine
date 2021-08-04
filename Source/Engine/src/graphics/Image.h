#pragma once

#include "Common.h"

namespace Quartz
{
	enum ImageType
	{
		IMAGE_TYPE_1D,
		IMAGE_TYPE_2D,
		IMAGE_TYPE_3D
	};

	enum ImageViewType
	{
		IMAGE_VIEW_TYPE_1D,
		IMAGE_VIEW_TYPE_2D,
		IMAGE_VIEW_TYPE_3D,
		IMAGE_VIEW_TYPE_CUBEMAP,
		IMAGE_VIEW_TYPE_ARRAY_1D,
		IMAGE_VIEW_TYPE_ARRAY_2D
	};

	enum ImageFormat
	{
		IMAGE_FORMAT_UNKNOWN,

		IMAGE_FORMAT_R,
		IMAGE_FORMAT_RG,
		IMAGE_FORMAT_RGB,
		IMAGE_FORMAT_RGBA,

		IMAGE_FORMAT_BGRA,

		IMAGE_FORMAT_R16,
		IMAGE_FORMAT_RG16,
		IMAGE_FORMAT_RGB16,
		IMAGE_FORMAT_RGBA16,

		IMAGE_FORMAT_R_FLOAT,
		IMAGE_FORMAT_RG_FLOAT,
		IMAGE_FORMAT_RGB_FLOAT,
		IMAGE_FORMAT_RGBA_FLOAT,

		IMAGE_FORMAT_DEPTH_32,
		IMAGE_FORMAT_DEPTH_24_STENCIL_8
	};

	enum ImageUsageBits
	{
		IMAGE_USAGE_INVALID							= 0x00,
		IMAGE_USAGE_SAMPLED_TEXTURE_BIT				= 0x01,
		IMAGE_USAGE_COLOR_ATTACHMENT_BIT			= 0x02,
		IMAGE_USAGE_DEPTH_ATTACHMENT_BIT			= 0x04,
		IMAGE_USAGE_STENCIL_ATTACHMENT_BIT			= 0x08,
		IMAGE_USAGE_INPUT_ATTACHMENT_BIT			= 0x10,
		IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT	= IMAGE_USAGE_DEPTH_ATTACHMENT_BIT | IMAGE_USAGE_STENCIL_ATTACHMENT_BIT,
		IMAGE_USAGE_TRANSFER_SRC_BIT				= 0x20,
		IMAGE_USAGE_TRANSFER_DST_BIT				= 0x40
	};

	typedef Flags32 ImageUsages;

	enum ImageViewUsage
	{
		IMAGE_VIEW_USAGE_INVALID,
		IMAGE_VIEW_USAGE_SAMPLED_TEXTURE,
		IMAGE_VIEW_USAGE_COLOR_ATTACHMENT,
		IMAGE_VIEW_USAGE_DEPTH_ATTACHMENT,
		IMAGE_VIEW_USAGE_STENCIL_ATTACHMENT,
		IMAGE_VIEW_USAGE_DEPTH_STENCIL_ATTACHMENT
	};

	class QUARTZ_API Image
	{
	protected:
		UInt32		mWidth;
		UInt32		mHeight;
		UInt32		mDepth;
		UInt32		mLayers;
		UInt32		mMips;
		ImageType	mType;
		ImageFormat mFormat;
		ImageUsages mUsages;

	protected:
		Image(ImageType type, UInt32 width, UInt32 height, UInt32 depth,
			UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages);

	public:
		FORCE_INLINE UInt32			GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32			GetHeight() const { return mHeight; }
		FORCE_INLINE UInt32			GetDepth() const { return mDepth; }
		FORCE_INLINE UInt32			GetLayers() const { return mLayers; }
		FORCE_INLINE UInt32			GetMips() const { return mMips; }
		FORCE_INLINE ImageType		GetType() const { return mType; }
		FORCE_INLINE ImageFormat	GetFormat() const { return mFormat; }
		FORCE_INLINE ImageUsages	GetUsages() const { return mUsages; }
	};
	
	class QUARTZ_API ImageView
	{
	protected:
		Image*			mpImage;
		UInt32			mWidth;
		UInt32			mHeight;
		UInt32			mDepth;
		UInt32			mLayers;
		UInt32			mLayerStart;
		UInt32			mMips;
		UInt32			mMipStart;
		ImageViewType	mType;
		ImageFormat		mFormat;
		ImageViewUsage	mImageViewUsage;

	protected:
		ImageView(Image* pImage, ImageViewType type,
			UInt32 width, UInt32 height, UInt32 depth,
			UInt32 layers, UInt32 layerStart, 
			UInt32 mips, UInt32 mipStart, ImageFormat format, 
			ImageViewUsage usage);

	public:
		FORCE_INLINE UInt32			GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32			GetHeight() const { return mHeight; }
		FORCE_INLINE UInt32			GetDepth() const { return mDepth; }
		FORCE_INLINE UInt32			GetLayers() const { return mLayers; }
		FORCE_INLINE UInt32			GetLayerStart() const { return mLayerStart; }
		FORCE_INLINE UInt32			GetMips() const { return mMips; }
		FORCE_INLINE UInt32			GetMipStart() const { return mMipStart; }
		FORCE_INLINE ImageViewType	GetViewType() const { return mType; }
		FORCE_INLINE ImageFormat	GetFormat() const { return mFormat; }
		FORCE_INLINE ImageViewUsage	GetViewUsage() const { return mImageViewUsage; }
	};
}