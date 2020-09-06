#pragma once

#include "Common.h"
#include "util\String.h"

#define DEFAULT_WINDOW_WIDTH	640
#define DEFAULT_WINDOW_HEIGHT	480
#define DEFAULT_WINDOW_TITLE	"Default Window"

namespace Quartz
{
	QUARTZ_API typedef void* WindowHandle;

	class QUARTZ_API OldWindow
	{
	protected:
		UInt32 mPosX;
		UInt32 mPosY;
		UInt32 mWidth;
		UInt32 mHeight;
		const char* mTitle;
		Bool8  mVisible;

		OldWindow() = default;

		virtual Bool8 ShowImpl() = 0;
		virtual Bool8 HideImpl() = 0;

		virtual Bool8 SetPositionImpl(UInt32 x, UInt32 y) = 0;
		virtual Bool8 SetSizeImpl(UInt32 width, UInt32 height) = 0;
		virtual Bool8 SetTitleImpl(const char* title) = 0;

		virtual WindowHandle GetNativeHandleImpl() = 0;

	public:

		virtual void Update() = 0;

		INLINE Bool8 Show();
		INLINE Bool8 Hide();

		INLINE Bool8 SetPosition(UInt32 x, UInt32 y);
		INLINE Bool8 SetSize(UInt32 width, UInt32 height);
		INLINE Bool8 SetTitle(const char* title);

		INLINE WindowHandle GetNativeHandle();

		FORCE_INLINE const char* GetTitle() const { return mTitle; }
		FORCE_INLINE UInt32 GetWidth() const { return mWidth; }
		FORCE_INLINE UInt32 GetHeight() const { return mWidth; }
	};
}