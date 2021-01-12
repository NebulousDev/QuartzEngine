#pragma once

#include "Win32.h"
#include "platform/VPWindow.h"

namespace Quartz
{
	class QUARTZ_API Win32Window : public VPWindow
	{
	public:
		friend class Win32Platform;

	private:
		HWND		mHWND;
		StringW		mTitle;
		Bounds2i	mBounds;
		Bounds2i	mClientBounds;
		Bool8		mMaximized : 1;
		Bool8		mMinimized : 1;
		Bool8		mFullscreen : 1;
		Bool8		mVisible : 1;

	public:
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

		const StringW& Title() override;
		Bounds2i Bounds() override;
		Bounds2i ClientBounds() override;
		Point2i Position() override;
		Point2i Size() override;
		Point2i ClientSize() override;
		Point2i Center() override;
		UInt32 Width() override;
		UInt32 Height() override;
		UInt32 ClientWidth() override;
		UInt32 ClientHeight() override;
		Bool8 IsMaximized() override;
		Bool8 IsMinimized() override;
		Bool8 IsFullscreen() override;
		Bool8 IsVisible() override;
	};
}