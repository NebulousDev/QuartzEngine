#pragma once

#include "Common.h"
#include "Entity.h"

namespace Quartz
{
	class EntityWorld;

	class SystemBase
	{
	public:
		friend class EntityWorld;

	protected:
		virtual void OnInit(EntityWorld& world) = 0;

		virtual void UpdateAll(EntityWorld& world, Float32 deltaTime) = 0;
		virtual void TickAll(EntityWorld& world, Float32 deltaTime) = 0;

		virtual void OnDestroy(EntityWorld& world) = 0;
	};

}