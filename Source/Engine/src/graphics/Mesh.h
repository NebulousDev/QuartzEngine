#pragma once

#include "VGFXContext.h"

namespace Quartz
{
	struct Material
	{
		int temp;
	};

	struct Mesh
	{
		HGFXBuffer	vertexBuffer;
		HGFXBuffer	indexBuffer;
		Material	material;
	};
}