#pragma once

#include "../Buffer.h"
#include "../../resource/Resource.h"

namespace Quartz
{
	struct QUARTZ_API Model : public Resource<Model>
	{
		Buffer* pVertexBuffer;
		Buffer* pIndexBuffer;
	};
}