#pragma once

#include "Common.h"

namespace Quartz
{
	struct Image
	{
		

		UInt32 width;
		UInt32 height;
		UInt32 depth;

		Byte* pData;
	};
}