#pragma once

#include "Win32.h"
#include "platform/Window.h"

namespace Quartz
{
	class QUARTZ_API Win32Window : public Window
	{
	public:
		friend class Win32WindowManager;

	private:
		HWND mHandle;

	public:
		Win32Window(HWND handle, const StringW& title, Bounds2i bounds, Bounds2i clientBounds, 
			Bool8 createdFullscreen, Bool8 createdMinimized, Bool8 createdMaximized, Bool8 createdVisible);

		void SetPosition(Int32 x, Int32 y) override;
		void SetClientPosition(Int32 x, Int32 y) override;
		void SetTitle(const StringW& title) override;
		void SetSize(UInt32 width, UInt32 height) override;
		void SetClientSize(UInt32 width, UInt32 height) override;
		void BringToFront() override;
		void Minimize() override;
		void Maximize() override;
		void Focus() override;
		void Show() override;
		void Hide() override;

		void* GetNativeHandle() override;

		HWND GetHWND() const;
	};
}