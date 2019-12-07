#pragma once

#include "util\Singleton.h"

namespace Quartz
{
	class QUARTZ_API Engine : public Singleton<Engine>
	{
	public:
		friend class Singleton<Engine>;

	private:
		Bool8 mInitialized;
		Bool8 mRunning;

		Engine() = default;

		void Run();
		void Update();

	public:
		void Start();
	};
}