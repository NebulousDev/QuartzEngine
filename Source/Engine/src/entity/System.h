#pragma once

#include "SystemBase.h"
#include "World.h"

namespace Quartz
{
	template<typename... Component>
	class EntitySystem : public SystemBase
	{
	private:
		void UpdateAll(EntityWorld& world, Float32 deltaTime) override
		{
			for (Entity entity : world.CreateView<Component...>())
			{
				OnUpdate(world, entity, deltaTime);
			}
		}

		void TickAll(EntityWorld& world, Float32 deltaTime) override
		{
			for (Entity entity : world.CreateView<Component...>())
			{
				OnTick(world, entity, deltaTime);
			}
		}

	public:
		virtual void OnInit(EntityWorld& world) {};
		virtual void OnUpdate(EntityWorld& world, Entity entity, Float32 deltaTime) {};
		virtual void OnTick(EntityWorld& world, Entity entity, Float32 deltaTime) {};
		virtual void OnDestroy(EntityWorld& world) {};
	};
}
