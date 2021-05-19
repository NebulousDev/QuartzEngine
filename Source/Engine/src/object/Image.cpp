#include "Image.h"

namespace Quartz
{
	Image::Image(UInt32 width, UInt32 height,
		UInt32 channels, UInt32 bitsPerChannel, Byte* data) :
		mWidth(width),
		mHeight(height),
		mChannels(channels),
		mBitsPerChannel(bitsPerChannel),
		mpData(data)
	{
		mSizeBytes = (width * height * channels * bitsPerChannel) / 8;
	}

	UInt32 Image::GetWidth() const
	{
		return mWidth;
	}

	UInt32 Image::GetHeight() const
	{
		return mHeight;
	}

	UInt32 Image::GetChannels() const
	{
		return mChannels;
	}

	UInt32 Image::GetBitsPerChannel() const
	{
		return mBitsPerChannel;
	}

	Byte* Image::GetData() const
	{
		return mpData;
	}

	UInt32 Image::GetSizeBytes() const
	{
		return mSizeBytes;
	}
}
