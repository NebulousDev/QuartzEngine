#pragma once

#include "../Module.h"

#include "Window.h"
#include "Application.h"
#include "PeripheralController.h"
#include "FileSystem.h"
#include "Time.h"

#include "DebugConsole.h"

namespace Quartz
{
	class QUARTZ_API Platform : public Module
	{
	protected:
		PeripheralController* mpPeripheralController;

	public:
		Platform(const ModuleInfo& info);

		virtual DebugConsole* CreateDebugConsole() = 0;
		virtual void DestroyDebugConsole(DebugConsole* pDebugConsole) = 0;

		virtual Application* CreateApplication(const ApplicationInfo& info) = 0;
		virtual Bool8 DestroyApplication(Application* application) = 0;

		virtual Time* GetTime() = 0;
		virtual PhysicalFileSystem* GetPhysicalFileSystem() = 0;

		FORCE_INLINE PeripheralController* GetPeripheralController() { return mpPeripheralController; }
	};
}