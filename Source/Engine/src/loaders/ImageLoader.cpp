#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace Quartz
{
	RawImage* LoadImageFunc(const String& path)
	{
		Int32 width, height, channels;

		stbi_uc* pPixels = stbi_load(path.Str(), &width, &height, &channels, STBI_rgb_alpha);//STBI_default);

		if (pPixels == nullptr)
		{
			return nullptr;
		}

		RawImage* pImage = new RawImage(width, height, 4/*channels*/, 8, pPixels);

		return pImage;
	}

	Bool8 UnloadImageFunc(RawImage* pImage)
	{
		if (pImage != nullptr)
		{
			stbi_image_free(pImage->GetData());
			delete pImage;
			return true;
		}

		return false;
	}
}

