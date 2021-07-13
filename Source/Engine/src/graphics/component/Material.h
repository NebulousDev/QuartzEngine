#pragma once

#include "util/String.h"
#include "../Image.h"

namespace Quartz
{
	struct MaterialComponent
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

		MaterialComponent(const String& diffuse,
			const String& normal,
			const String& roughness,
			const String& metallic,
			const String& ambient);
	};
}