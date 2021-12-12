#pragma once

#include "platform/Platform.h"
#include "graphics/Surface.h"
#include "Win32PlatformConsole.h"
#include "Win32Window.h"
#include "Win32FileSystem.h"
#include "Win32Time.h"

namespace Quartz
{
	class QUARTZ_API Win32Platform : public Platform
	{
	private:
		HINSTANCE			mInstance;
		Win32Time			mTime;
		Win32FileSystem*	mpFileSystem;

	public:
		Win32Platform();

		Bool8 Init() override;
		Bool8 PostInit() override;
		void Shutdown() override;

		void Update(Float32 delta) override;

		DebugConsole* CreateDebugConsole() override;
		void DestroyDebugConsole(DebugConsole* pDebugConsole) override;

		Application* CreateApplication(const ApplicationInfo& info) override;
		Bool8 DestroyApplication(Application* application) override;

		Time* GetTime() override;
		PhysicalFileSystem* GetPhysicalFileSystem() override;
	};
}