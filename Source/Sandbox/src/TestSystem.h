#pragma once

#include "entity/System.h"
#include "math/Vector3.h"
#include <stdio.h>

namespace Quartz
{
	struct TestComponent
	{
		UInt32 value;
		inline TestComponent(UInt32 value) : value(value) {};
	};

	class TestSystem : public EntitySystem<TestComponent>
	{
	public:
		inline void OnInit(EntityWorld& world) override
		{
			printf("Init!\n");
		}

		inline void OnUpdate(EntityWorld& world, Entity entity, Float32 deltaTime) override
		{
			TestComponent& test = world.GetComponent<TestComponent>(entity);
			printf("Update! | Entity=%d | TestVal = %d\n", entity.index, test.value);
		}

		inline void OnTick(EntityWorld& world, Entity entity, Float32 deltaTime) override
		{
			TestComponent& test = world.GetComponent<TestComponent>(entity);
			printf("Update! | Entity=%d | TestVal = %d\n", entity.index, test.value);
		}

		inline void OnDestroy(EntityWorld& world) override
		{
			printf("Destroy!\n");
		}
	};
}