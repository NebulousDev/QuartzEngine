#pragma once

#include "Common.h"
#include "platform/Window.h"

namespace Quartz
{
	typedef Int64 Win32WindowHandle;

	class QUARTZ_API Win32Window : public Window
	{
	private:
		Win32WindowHandle mHandle;

	private:
		Bool8 ShowImpl() override;
		Bool8 HideImpl() override;

		Bool8 SetPositionImpl(UInt32 xPos, UInt32 yPos) override;
		Bool8 SetSizeImpl(UInt32 width, UInt32 height) override;
		Bool8 SetTitleImpl(const char* title) override;

		WindowHandle GetNativeHandleImpl() override;

	public:

		void Update() override;

		Win32Window();
		Win32Window(UInt32 width, UInt32 height, const char* title);
		Win32Window(UInt32 x, UInt32 y, UInt32 width, UInt32 height, const char* title);
	};
}