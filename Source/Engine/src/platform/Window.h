#pragma once

#include "Common.h"
#include "util/String.h"
#include "math/Bounds.h"

namespace Quartz
{
	class QUARTZ_API Window
	{
	protected:
		StringW		mTitle;
		Bounds2i	mBounds;
		Bounds2i	mClientBounds;
		Bool8		mMaximized : 1;
		Bool8		mMinimized : 1;
		Bool8		mFullscreen : 1;
		Bool8		mVisible : 1;

		Window(const StringW& title, Bounds2i bounds, Bounds2i clientBounds, Bool8 createdFullscreen, 
			Bool8 createdMinimized, Bool8 createdMaximized, Bool8 createdVisible);

	public:
		virtual void SetPosition(Int32 x, Int32 y) = 0;
		virtual void SetClientPosition(Int32 x, Int32 y) = 0;
		virtual void SetTitle(const StringW& title) = 0;
		virtual void SetSize(UInt32 width, UInt32 height) = 0;
		virtual void SetClientSize(UInt32 width, UInt32 height) = 0;
		virtual void BringToFront() = 0;
		virtual void Minimize() = 0;
		virtual void Maximize() = 0;
		virtual void Focus() = 0;
		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual void* GetNativeHandle() = 0;

		const StringW& Title();
		Bounds2i Bounds();
		Bounds2i ClientBounds();
		Point2i Position();
		Point2i Size();
		Point2i ClientSize();
		Point2i Center();
		UInt32 Width();
		UInt32 Height();
		UInt32 ClientWidth();
		UInt32 ClientHeight();
		Bool8 IsMaximized();
		Bool8 IsMinimized();
		Bool8 IsFullscreen();
		Bool8 IsVisible();
	};
}