#pragma once

#include "../Common.h"

namespace Quartz
{
	template<typename ClassType, typename... CreateArgs>
	class Singleton
	{
	private:
		static ClassType* spInstance;

	protected:
		virtual ClassType* CreateInstanceImpl(CreateArgs... args) = 0;

	public:
		static ClassType* CreateInstance(CreateArgs... args)
		{
			using SingletonClass = Singleton<ClassType, CreateArgs...>;

			if (spInstance == nullptr)
			{
				ClassType dummyInstance;
				spInstance = dummyInstance.CreateInstanceImpl(args...);
			}

			return spInstance;
		}

		static ClassType* GetInstance()
		{
			return spInstance;
		}
	};

	template<typename ClassType, typename... CreateArgs>
	ClassType* Singleton<ClassType, CreateArgs...>::spInstance = nullptr;
}