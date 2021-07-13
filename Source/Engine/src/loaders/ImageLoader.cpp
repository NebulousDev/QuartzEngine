#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace Quartz
{
	RawImage* LoadImage(const String& path)
	{
		Int32 width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* pPixels = stbi_load(path.Str(), &width, &height, &channels, STBI_rgb_alpha);//STBI_default);

		if (pPixels == nullptr)
		{
			return nullptr;
		}

		RawImage* pImage = new RawImage(width, height, 4/*channels*/, 8, pPixels);

		return pImage;
	}

	void FreeImage(RawImage* pImage)
	{
		if (pImage != nullptr)
		{
			stbi_image_free(pImage->GetData());
			delete pImage;
		}
	}
}

