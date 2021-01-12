#include "Window.h"

#include "../platform/VPWindow.h"

namespace Quartz
{
	void GraphicsWindow::Show() const
	{
		static_cast<VPWindow*>(window)->Show();
	}

	void GraphicsWindow::Focus() const
	{
		static_cast<VPWindow*>(window)->Focus();
	}
}

