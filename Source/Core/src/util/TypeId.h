#pragma once

#include "../Common.h"

namespace Quartz
{
	typedef UInt64 TypeId;

	template <typename Type>
	struct TypeIdValue
	{
		static char sId;
	};

	template<typename Type>
	constexpr TypeId GetTypeId()
	{
		return reinterpret_cast<TypeId>(&TypeIdValue<Type>::sId);
	}

	template <typename Type>
	char TypeIdValue<Type>::sId;
}