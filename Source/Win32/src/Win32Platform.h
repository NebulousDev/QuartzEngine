#pragma once

#include "platform/Platform.h"
#include "graphics/Surface.h"
#include "Win32PlatformConsole.h"
#include "Win32Window.h"
#include "Win32Time.h"

#include "util/Map.h"

namespace Quartz
{
	// TODO: TEMP, REMOVE
	struct Win32InputDeviceInfo
	{
		StringW id;
		StringW parentId;
		StringW name;
		StringW vendor;
		StringW type;
		StringW fullPath;
		UInt32 usagePage;
		UInt32 usage;
	};

	class QUARTZ_API Win32Platform : public Platform
	{
	private:
		HINSTANCE	mInstance;
		Win32Time	mTime;

	public:
		Win32Platform();

		DebugConsole* CreateDebugConsole() override;
		void DestroyDebugConsole(DebugConsole* pDebugConsole) override;

		Application* CreateApplication(const ApplicationInfo& info) override;
		Bool8 DestroyApplication(Application* application) override;

		Time* GetTime() override;
	};
}