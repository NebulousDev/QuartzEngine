#include "PlatformWindow.h"

namespace Quartz
{
	Bool8 OldWindow::Show()
	{
		if (ShowImpl())
		{
			mVisible = true;
			return true;
		}

		return false;
	}

	Bool8 OldWindow::Hide()
	{
		if (HideImpl())
		{
			mVisible = false;
			return true;
		}

		return false;
	}

	Bool8 OldWindow::SetPosition(UInt32 xPos, UInt32 yPos)
	{
		if (SetPositionImpl(xPos, yPos))
		{
			mPosX = xPos;
			mPosY = yPos;
			return true;
		}

		return false;
	}

	Bool8 OldWindow::SetSize(UInt32 width, UInt32 height)
	{
		if (SetSizeImpl(width, height))
		{
			mWidth = width;
			mHeight = height;
			return true;
		}

		return false;
	}

	Bool8 OldWindow::SetTitle(const char* title)
	{
		if (SetTitleImpl(title))
		{
			mTitle = title;
			return true;
		}

		return false;
	}

	WindowHandle OldWindow::GetNativeHandle()
	{
		return GetNativeHandleImpl();
	}
}
