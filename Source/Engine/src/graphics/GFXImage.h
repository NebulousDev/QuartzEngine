#pragma once

#include "Common.h"

#include "GFXResource.h"
#include "GFXFormat.h"

namespace Quartz
{
	enum GFXImageType
	{
		IMAGE_TYPE_1D,
		IMAGE_TYPE_2D,
		IMAGE_TYPE_3D
	};

	class QUARTZ_API GFXImage : public GFXResource
	{

	};

	class QUARTZ_API GFXImageView : public GFXResource
	{

	};
}