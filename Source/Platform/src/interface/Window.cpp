#include "Window.h"

namespace Quartz
{
	Bool8 Window::Show()
	{
		if (ShowImpl())
		{
			mVisible = true;
			return true;
		}

		return false;
	}

	Bool8 Window::Hide()
	{
		if (HideImpl())
		{
			mVisible = false;
			return true;
		}

		return false;
	}

	Bool8 Window::SetPosition(UInt32 xPos, UInt32 yPos)
	{
		if (SetPositionImpl(xPos, yPos))
		{
			mPosX = xPos;
			mPosY = yPos;
			return true;
		}

		return false;
	}

	Bool8 Window::SetSize(UInt32 width, UInt32 height)
	{
		if (SetSizeImpl(width, height))
		{
			mWidth = width;
			mHeight = height;
			return true;
		}

		return false;
	}

	Bool8 Window::SetTitle(const char* title)
	{
		if (SetTitleImpl(title))
		{
			mTitle = title;
			return true;
		}

		return false;
	}

	WindowHandle Window::GetNativeHandle()
	{
		return GetNativeHandleImpl();
	}
}
