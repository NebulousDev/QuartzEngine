#include "Material.h"

#include "../../Engine.h"
#include "../../log/Log.h"
#include "../../loaders/ImageLoader.h"

#include <iostream>
#include <fstream>
#include <math.h>

namespace Quartz
{
	static Array<Byte> ReadFile(const String& filename)
	{
		std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			Log::Error("Cannot open file %s", filename.Str());
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		Array<Byte> buffer(fileSize);

		file.seekg(0);
		file.read((char*)buffer.Data(), fileSize);

		file.close();

		return buffer;
	}

	void CreateMaterialTexture(const String& path, Image** ppImageOut, ImageView** ppViewOut)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		RawImage* pRawImage = LoadImage(path, true, true);

		if (!pRawImage)
		{
			Log::Error("Failed to load image path '%s;", path.Str());
			return;
		}

		ImageFormat format;

		if (pRawImage->GetBitsPerChannel() == 8)
		{
			switch (pRawImage->GetChannels())
			{
				case 1: format = IMAGE_FORMAT_R;	break;
				case 2: format = IMAGE_FORMAT_RG;	break;
				case 3: format = IMAGE_FORMAT_RGB;	break;
				case 4: format = IMAGE_FORMAT_RGBA; break;
			}
		}
		else if (pRawImage->GetBitsPerChannel() == 16)
		{
			switch (pRawImage->GetChannels())
			{
				case 1: format = IMAGE_FORMAT_R16;		break;
				case 2: format = IMAGE_FORMAT_RG16;		break;
				case 3: format = IMAGE_FORMAT_RGB16;	break;
				case 4: format = IMAGE_FORMAT_RGBA16;	break;
			}
		}
		else if (pRawImage->GetBitsPerChannel() == 32)
		{
			switch (pRawImage->GetChannels())
			{
				case 1: format = IMAGE_FORMAT_R_FLOAT;		break;
				case 2: format = IMAGE_FORMAT_RG_FLOAT;		break;
				case 3: format = IMAGE_FORMAT_RGB_FLOAT;	break;
				case 4: format = IMAGE_FORMAT_RGBA_FLOAT;	break;
			}
		}
		else
		{
			// TODO: ERROR
			*ppImageOut = nullptr;
			*ppViewOut	= nullptr;
			return;
		}

		UInt32 mipLevels = static_cast<uint32_t>(std::floor(std::log2f(std::max(pRawImage->GetWidth(), pRawImage->GetHeight())))) + 1;
		UInt32 sizeBytes = pRawImage->GetSizeBytes();

		Image* pImage = pGraphics->CreateImage
		(
			IMAGE_TYPE_2D,
			pRawImage->GetWidth(), pRawImage->GetHeight(), 1, 1, mipLevels,
			format,
			IMAGE_USAGE_SAMPLED_TEXTURE_BIT | IMAGE_USAGE_TRANSFER_SRC_BIT | IMAGE_USAGE_TRANSFER_DST_BIT
		);

		Buffer* pStagingBuffer = pGraphics->CreateBuffer
		(
			sizeBytes,
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT
		);

		void* pStagingData = pStagingBuffer->MapBuffer(sizeBytes, 0);
		memcpy(pStagingData, pRawImage->GetData(), sizeBytes);
		pStagingBuffer->UnmapBuffer();

		pGraphics->CopyBufferToImage(pStagingBuffer, pImage);

		pGraphics->DestroyBuffer(pStagingBuffer);

		pGraphics->GenerateMips(pImage);

		ImageView* pImageView = pGraphics->CreateImageView
		(
			pImage,
			IMAGE_VIEW_TYPE_2D,
			pRawImage->GetWidth(), pRawImage->GetHeight(), 1, 1, 0, mipLevels, 0,
			format,
			IMAGE_VIEW_USAGE_SAMPLED_TEXTURE
		);

		FreeImage(pRawImage);

		*ppImageOut = pImage;
		*ppViewOut = pImageView;
	}

	// TODO: Temporary until proper material management
	MaterialComponent::MaterialComponent(
		const String& diffuse, 
		const String& normal, 
		const String& roughness,
		const String& metallic,
		const String& ambient)
	{
		CreateMaterialTexture(diffuse, &pDiffuseImage, &pDiffuse);
		CreateMaterialTexture(normal, &pNormalImage, &pNormal);
		CreateMaterialTexture(roughness, &pRoughnessImage, &pRoughness);
		CreateMaterialTexture(metallic, &pMetallicImage, &pMetallic);
		CreateMaterialTexture(ambient, &pAmbientImage, &pAmbient);
	}
}

