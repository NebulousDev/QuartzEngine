#pragma once

#include "Window.h"
#include "../event/Event.h"
#include "util/Array.h"

namespace Quartz
{
	struct WindowCloseEvent : public Event<WindowCloseEvent>
	{
		Window* pWindow;
	};

	struct ApplicationInfo
	{
		StringW name;
		StringW version;
	};

	class QUARTZ_API Application
	{
	protected:
		ApplicationInfo mInfo;
		Array<Window*> mWindows;

	public:
		Application(const ApplicationInfo& info);

		virtual Window* CreateWindow(const WindowInfo& info) = 0;

		virtual void PollMessages() = 0;
	};
}