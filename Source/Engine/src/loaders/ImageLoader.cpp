#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace Quartz
{
    Image LoadImageSTB(const String& filepath)
    {
        Int32 width, height, channels;
        stbi_uc* pPixels = stbi_load(filepath.Str(), &width, &height, &channels, STBI_rgb_alpha);

        Image image;
        image.width = width;
        image.height = height;
        image.pData = pPixels;

        return image;
    }
}

