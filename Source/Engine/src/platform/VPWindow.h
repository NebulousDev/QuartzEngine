#pragma once

#include "Common.h"
#include "util/String.h"
#include "math/Bounds.h"

#include "VPPlatform.h"

namespace Quartz
{
	class QUARTZ_API VPWindow : public VPResource
	{
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

		virtual const StringW& Title() = 0;
		virtual Bounds2i Bounds() = 0;
		virtual Bounds2i ClientBounds() = 0;
		virtual Point2i Position() = 0;
		virtual Point2i Size() = 0;
		virtual Point2i ClientSize() = 0;
		virtual Point2i Center() = 0;
		virtual UInt32 Width() = 0;
		virtual UInt32 Height() = 0;
		virtual UInt32 ClientWidth() = 0;
		virtual UInt32 ClientHeight() = 0;
		virtual Bool8 IsMaximized() = 0;
		virtual Bool8 IsMinimized() = 0;
		virtual Bool8 IsFullscreen() = 0;
		virtual Bool8 IsVisible() = 0;
	};
}