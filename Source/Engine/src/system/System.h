#pragma once

#include "Common.h"

namespace Quartz
{
	class QUARTZ_API System
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Tick() = 0;
		virtual void Destroy() = 0;
	};
}