#pragma once

#include "../Module.h"

#include "Window.h"
#include "Application.h"
#include "Time.h"

#include "DebugConsole.h"

namespace Quartz
{
	class QUARTZ_API Platform : public Module
	{
	public:
		Platform(const ModuleInfo& info);

		virtual DebugConsole* CreateDebugConsole() = 0;
		virtual void DestroyDebugConsole(DebugConsole* pDebugConsole) = 0;

		virtual Application* CreateApplication(const ApplicationInfo& info) = 0;
		virtual Bool8 DestroyApplication(Application* application) = 0;

		virtual Time* GetTime() = 0;
	};
}