#pragma once

#include "../Common.h"

namespace Quartz
{
	template<typename ClassType>
	class Singleton
	{
	protected:
		static ClassType smInstance;

	public:
		static ClassType& GetInstanceHandle()
		{
			return smInstance;
		}
	};

	template<typename ClassType>
	ClassType Singleton<ClassType>::smInstance;
}