#pragma once
 
#include "../Resource.h"

namespace Quartz
{
	class QUARTZ_API RawImage : public Resource<RawImage>
	{
	private:
		UInt32		mWidth;
		UInt32		mHeight;
		UInt32		mChannels;
		UInt32		mBitsPerChannel;
		UInt32		mSizeBytes;
		Bool8		mHDR;

		Byte*		mpData;

	public:
		RawImage(UInt32 width, UInt32 height, 
			UInt32 channels, UInt32 bitsPerChannel, Bool8 hdr, Byte* data);

		UInt32	GetWidth() const;
		UInt32	GetHeight() const;
		UInt32	GetChannels() const;
		UInt32	GetBitsPerChannel() const;
		UInt32	GetSizeBytes() const;
		Byte*	GetData() const;
		Bool8	IsHDR() const;
	};
}