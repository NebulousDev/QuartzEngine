#pragma once

#include "../Module.h"
#include "util/Array.h"
#include "../platform/Application.h"

namespace Quartz
{
	class QUARTZ_API ApplicationManager : public Module
	{
	private:
		Array<Application*> mApps;

	public:
		ApplicationManager();

		Application* CreateManagedApplication(const ApplicationInfo& info);

		void Tick() override;
	};
}