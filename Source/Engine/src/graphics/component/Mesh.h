#pragma once

#include "../Buffer.h"
#include "../../object/Model.h"

namespace Quartz
{
	struct MeshComponent
	{
		String	filepath;
		Buffer* pVertexBuffer;
		Buffer* pIndexBuffer;

		MeshComponent(const String& filepath);
	};
}