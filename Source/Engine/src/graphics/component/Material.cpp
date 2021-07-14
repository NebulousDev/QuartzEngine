#include "Material.h"

#include "../../Engine.h"
#include "../../log/Log.h"
#include "../../loaders/ImageLoader.h"

#include <iostream>
#include <fstream>

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

		RawImage* pRawImage = LoadImage(path);

		if (!pRawImage)
		{
			Log::Error("Failed to load image path '%s;", path.Str());
			return;
		}

		Image* pImage = pGraphics->CreateImage
		(
			IMAGE_TYPE_2D,
			pRawImage->GetWidth(), pRawImage->GetHeight(), 1, 1, 1,
			IMAGE_FORMAT_RGBA,
			IMAGE_USAGE_SAMPLED_TEXTURE_BIT | IMAGE_USAGE_TRANSFER_DST_BIT
		);

		UInt32 sizeBytes = pRawImage->GetWidth() * pRawImage->GetHeight() * 4; // temp RGBA only
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

		ImageView* pImageView = pGraphics->CreateImageView
		(
			pImage,
			IMAGE_VIEW_TYPE_2D,
			pRawImage->GetWidth(), pRawImage->GetHeight(), 1, 1, 0, 1, 0,
			IMAGE_FORMAT_RGBA,
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

