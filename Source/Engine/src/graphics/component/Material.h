#pragma once

#include "util/String.h"
#include "../Image.h"

namespace Quartz
{
	struct MaterialComponent
	{
		Image* pDiffuseImage; // temp
		ImageView* pDiffuse;

		MaterialComponent(const String& diffuse);
	};
}