#pragma once

#include "../graphics/Image.h"

namespace Quartz
{
	class QUARTZ_API RawImage
	{
	private:
		UInt32		mWidth;
		UInt32		mHeight;
		UInt32		mChannels;
		UInt32		mBitsPerChannel;
		UInt32		mSizeBytes;

		Byte*		mpData;

	public:
		RawImage(UInt32 width, UInt32 height, 
			UInt32 channels, UInt32 bitsPerChannel, Byte* data);

		UInt32	GetWidth() const;
		UInt32	GetHeight() const;
		UInt32	GetChannels() const;
		UInt32	GetBitsPerChannel() const;
		UInt32	GetSizeBytes() const;
		Byte*	GetData() const;
	};
}