#pragma once

#include "../Common.h"

namespace Quartz
{
	template<typename Type>
	class Singleton
	{
	public:
		INLINE static Type& GetInstance();
	};

	template<typename Type>
	Type& Singleton<Type>::GetInstance()
	{
		static Type sInstance;
		return sInstance;
	}
}