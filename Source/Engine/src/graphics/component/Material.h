#pragma once

#include "../../entity/Component.h"
#include "util/String.h"
#include "../Image.h"

#include "../../resource/object/Material.h"

namespace Quartz
{
	struct MaterialComponent : public Component<MaterialComponent>
	{
		Image* pDiffuseImage;	// temp
		Image* pNormalImage;	// temp
		Image* pRoughnessImage; // temp
		Image* pMetallicImage;	// temp
		Image* pAmbientImage;	// temp
		ImageView* pDiffuse;
		ImageView* pNormal;
		ImageView* pRoughness;
		ImageView* pMetallic;
		ImageView* pAmbient;

		Material* pMaterial;

		MaterialComponent(const String& diffuse,
			const String& normal,
			const String& roughness,
			const String& metallic,
			const String& ambient,
			Material* pMaterial);
	};
}