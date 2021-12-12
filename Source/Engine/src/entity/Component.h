#pragma once

#include "util/TypeId.h"

namespace Quartz
{
	struct QUARTZ_API ComponentBase
	{
		virtual UInt64 GetComponentId() = 0;
		virtual UInt64 GetComponentIndex() = 0;
	};

	template<typename ComponentType>
	struct QUARTZ_API Component : public ComponentBase
	{
		static constexpr UInt64 COMPONENT_ID	= TypeId<ComponentType>::Value();
		static constexpr UInt64 COMPONENT_INDEX	= TypeIndex<Component, ComponentType>::Value();

		UInt64 GetComponentId() override
		{
			return COMPONENT_ID;
		}

		UInt64 GetComponentIndex() override
		{
			return COMPONENT_INDEX;
		}
	};
}