#include "Material.h"

#include "../../Engine.h"
#include "../../log/Log.h"
#include "../../resource/loaders/ImageLoader.h"

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
		Graphics*			pGraphics			= Engine::GetInstance()->GetGraphics();
		ResourceManager*	pResourceManager	= Engine::GetInstance()->GetResourceManager();

		Image* pImage = pResourceManager->CheckoutResource<Image>(path);

		if (!pImage)
		{
			Log::Error("Failed to create material texture: GetResource() failed on path '%s'!", path.Str());
			return;
		}

		ImageView* pImageView = pGraphics->CreateImageView
		(
			pImage,
			IMAGE_VIEW_TYPE_2D,
			pImage->GetWidth(), pImage->GetHeight(), 1, 1, 0, pImage->GetMips(), 0,
			pImage->GetFormat(),
			IMAGE_VIEW_USAGE_SAMPLED_TEXTURE
		);

		*ppImageOut = pImage;
		*ppViewOut	= pImageView;
	}

	// TODO: Temporary until proper material management
	MaterialComponent::MaterialComponent(
		const String& diffuse, 
		const String& normal, 
		const String& roughness,
		const String& metallic,
		const String& ambient,
		Material* pMaterial)
	{
		CreateMaterialTexture(diffuse, &pDiffuseImage, &pDiffuse);
		CreateMaterialTexture(normal, &pNormalImage, &pNormal);
		CreateMaterialTexture(roughness, &pRoughnessImage, &pRoughness);
		CreateMaterialTexture(metallic, &pMetallicImage, &pMetallic);
		CreateMaterialTexture(ambient, &pAmbientImage, &pAmbient);
		this->pMaterial = pMaterial;
	}
}

