#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace Quartz
{
	RawImage* LoadImageSTB(const String& path, Bool8 flipY, Bool8 forceRGBA)
	{
		RawImage*	pImage = nullptr;

		Int32		width, height, channels;
		UInt32		bitsPerChannel;
		Bool8		isHDR;
		Byte*		pPixels;

		UInt32		requiredChannels = forceRGBA ? STBI_rgb_alpha : STBI_default;

		stbi_set_flip_vertically_on_load(flipY);
		
		if (stbi_is_hdr(path.Str()))
		{
			pPixels = (Byte*)stbi_loadf(path.Str(), &width, &height, &channels, requiredChannels);
			bitsPerChannel = 32;
			isHDR = true;
		}
		else if (stbi_is_16_bit(path.Str()))
		{
			pPixels = (Byte*)stbi_load_16(path.Str(), &width, &height, &channels, requiredChannels);
			bitsPerChannel = 16;
			isHDR = false;
		}
		else
		{
			pPixels = stbi_load(path.Str(), &width, &height, &channels, requiredChannels);
			bitsPerChannel = 8;
			isHDR = false;
		}

		if (pPixels == nullptr)
		{
			return nullptr;
		}
		
		if (requiredChannels == STBI_rgb_alpha)
		{
			pImage = new RawImage(width, height, 4, bitsPerChannel, isHDR, pPixels);
		}
		else
		{
			pImage = new RawImage(width, height, channels, bitsPerChannel, isHDR, pPixels);
		}

		return pImage;
	}

	void FreeImageSTB(RawImage* pImage)
	{
		if (pImage != nullptr)
		{
			stbi_image_free(pImage->GetData());
			delete pImage;
		}
	}
}

