#pragma once

#include "Common.h"

namespace Quartz
{
	enum ImageFormat
	{
		IMAGE_FORMAT_R,
		IMAGE_FORMAT_RG,
		IMAGE_FORMAT_RGB,
		IMAGE_FORMAT_RGBA,

		IMAGE_FORMAT_BGRA,

		IMAGE_FORMAT_R_FLOAT,
		IMAGE_FORMAT_RG_FLOAT,
		IMAGE_FORMAT_RGB_FLOAT,
		IMAGE_FORMAT_RGBA_FLOAT,
		IMAGE_FORMAT_DEPTH_24,
		IMAGE_FORMAT_DEPTH_24_STENCIL_8
	};

	class QUARTZ_API Image
	{
	private:
		UInt32		mWidth;
		UInt32		mHeight;
		UInt32		mChannels;
		UInt32		mBitsPerChannel;
		UInt32		mSizeBytes;

		Byte* mpData;

	public:
		Image(UInt32 width, UInt32 height, 
			UInt32 channels, UInt32 bitsPerChannel, Byte* data);

		UInt32 GetWidth() const;
		UInt32 GetHeight() const;
		UInt32 GetChannels() const;
		UInt32 GetBitsPerChannel() const;
		UInt32 GetSizeBytes() const;
		Byte* GetData() const;
	};
}