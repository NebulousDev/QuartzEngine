#pragma once

#include "Common.h"

#include "GFXResource.h"

namespace Quartz
{
	class QUARTZ_API GFXDevice : public GFXResource
	{
	private:
		int temp;
	public:
		GFXDevice() = default;
	};
}