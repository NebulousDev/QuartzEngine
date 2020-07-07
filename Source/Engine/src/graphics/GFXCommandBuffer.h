#pragma once

#include "Common.h"
#include "GFXResource.h"

namespace Quartz
{
	class QUARTZ_API GFXCommandBuffer : public GFXResource
	{
	private:

	public:
		GFXCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
	};
}