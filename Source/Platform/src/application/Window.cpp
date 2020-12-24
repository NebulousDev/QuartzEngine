
#include "Window.h"

namespace Quartz
{
	Window::Window(const StringW& title, Bounds2i bounds, Bounds2i clientBounds, Bool8 createdFullscreen,
		Bool8 createdMinimized, Bool8 createdMaximized, Bool8 createdVisible)
		: mTitle(title), mBounds(bounds), mClientBounds(clientBounds), mFullscreen(createdFullscreen), 
		mMinimized(createdMinimized), mMaximized(createdMaximized), mVisible(createdVisible) {}

	const StringW& Window::Title()
	{
		return mTitle;
	}

	Bounds2i Window::Bounds()
	{
		return mBounds;
	}

	Bounds2i Window::ClientBounds()
	{
		return mClientBounds;
	}

	Point2i Window::Position()
	{
		return mBounds.start;
	}

	Point2i Window::Size()
	{
		return Point2i(Width(), Height());
	}

	Point2i Window::ClientSize()
	{
		return Point2i(ClientWidth(), ClientHeight());
	}

	Point2i Window::Center()
	{
		return Point2i(mClientBounds.start.x + mClientBounds.Width() / 2,
			mClientBounds.start.y + mClientBounds.Height() / 2);
	}

	UInt32 Window::Width()
	{
		return mBounds.Width();
	}

	UInt32 Window::Height()
	{
		return mBounds.Height();
	}

	UInt32 Window::ClientWidth()
	{
		return mClientBounds.Width();
	}

	UInt32 Window::ClientHeight()
	{
		return mClientBounds.Height();
	}

	Bool8 Window::IsMaximized()
	{
		return mMaximized;
	}

	Bool8 Window::IsMinimized()
	{
		return mMinimized;
	}

	Bool8 Window::IsFullscreen()
	{
		return mFullscreen;
	}

	Bool8 Window::IsVisible()
	{
		return mVisible;
	}
}