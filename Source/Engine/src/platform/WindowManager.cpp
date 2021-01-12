#include "WindowManager.h"

namespace Quartz
{
	void WindowManager::SetFocusLostCallback(WindowFocusLostCallback callback)
	{
		mFocusLostCallback = callback;
	}

	void WindowManager::SetFocusGainedCallback(WindowFocusGainedCallback callback)
	{
		mFocusGainedCallback = callback;
	}

	void WindowManager::SetResizedCallback(WindowResizedCallback callback)
	{
		mResizedCallback = callback;
	}

	void WindowManager::SetMovedCallback(WindowMovedCallback callback)
	{
		mMovedCallback = callback;
	}

	WindowFocusLostCallback WindowManager::GetFocusLostCallback()
	{
		return mFocusLostCallback;
	}

	WindowFocusGainedCallback WindowManager::GetFocusGainedCallback()
	{
		return mFocusGainedCallback;
	}

	WindowResizedCallback WindowManager::GetResizedCallback()
	{
		return mResizedCallback;
	}

	WindowMovedCallback WindowManager::GetMovedCallback()
	{
		return mMovedCallback;
	}

	Window* WindowManager::GetActiveWindow()
	{
		return mpActiveWindow;
	}
}

