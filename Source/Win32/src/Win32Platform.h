#pragma once

#include "platform/VPPlatform.h"
#include "graphics2/Surface.h"
#include "Win32PlatformConsole.h"
#include "Win32Window.h"

#include "util/Map.h"

namespace Quartz
{
	struct Win32InputDeviceInfo
	{
		StringW id;
		StringW parentId;
		StringW name;
		StringW vendor;
		StringW type;
		StringW fullPath;
		UInt32 usagePage;
		UInt32 usage;
	};

	struct Win32InputMouse : public VPInputMouse
	{
		HANDLE hDevice;
		Win32InputDeviceInfo info;
	};

	struct Win32InputKeyboard : public VPInputMouse
	{
		HANDLE hDevice;
		Win32InputDeviceInfo info;
	};

	struct Win32InputController : public VPInputMouse
	{
		HANDLE hDevice;
		Win32InputDeviceInfo info;
	};

	typedef StringW DeviceId;

	class QUARTZ_API Win32Platform : public Platform
	{
	private:
		HINSTANCE mInstance;

		VPWindowMovedCallback			mWindowMovedCallback;
		VPWindowResizedCallback			mWindowResizedCallback;
		VPWindowFocusCallback			mWindowFocusCallback;
		VPWindowMouseEnteredCallback	mWindowMouseEnteredCallback;

		VPMouseMoveCallbackFunc			mVPMouseMoveCallbackFunc;
		VPMouseButtonCallbackFunc		mVPMouseButtonCallbackFunc;
		VPKeyboardKeyCallbackFunc		mVPKeyboardKeyCallbackFunc;

		// @Note: I cannot use HWND here as I would like
		//	because void* cannot be used in a hash function
		//	for whatever reason...
		Map<Handle64, Win32Window*>		mHWNDMap;
		Map<Handle64, DeviceId>			mDeviceIds;

		Map<DeviceId, Win32InputMouse*>			mMice;
		Map<DeviceId, Win32InputKeyboard*>		mKeyboards;
		Map<DeviceId, Win32InputController*>	mControllers;

		Win32Window* mCapturingWindow;

	private:
		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		DebugConsole* CreateDebugConsole() override;
		void DestroyDebugConsole(DebugConsole* pDebugConsole) override;

		Application* CreateApplication(const ApplicationInfo& info) override;
		Bool8 DestroyApplication(Application* application) override;

		/*
		HVPWindow CreateWindow(UInt32 posX, UInt32 posY,
			UInt32 clientWidth, UInt32 clientHeight, const StringW& title) override;
		void DestroyWindow(HVPWindow window) override;

		void MoveWindow(HVPWindow window, UInt32 x, UInt32 y) override;
		void ResizeWindow(HVPWindow window, UInt32 width, UInt32 height) override;
		void FocusWindow(HVPWindow window) override;
		void ShowWindow(HVPWindow window, Bool8 show) override;

		void SetWindowMovedCallback(VPWindowMovedCallback callback) override;
		void SetWindowResizedCallback(VPWindowResizedCallback callback) override;
		void SetWindowFocusCallback(VPWindowFocusCallback callback) override;
		void SetWindowMouseEnteredCallback(VPWindowMouseEnteredCallback callback) override;

		void SetMouseMoveCallback(VPMouseMoveCallbackFunc callback) override;
		void SetMouseButtonCallback(VPMouseButtonCallbackFunc callback) override;
		void SetKeyboardKeyCallback(VPKeyboardKeyCallbackFunc callback) override;

		void ShowCursor(Bool8 shown) override;
		void CaptureCursor(HVPWindow window) override;
		void ReleaseCursor() override;

		HVPWindow GetCapturingWindow() override;

		Point2i GetCursorPosition() override;

		Point2i GetWindowPosition(HVPWindow window) override;
		Point2i GetWindowClientPosition(HVPWindow window) override;
		Point2i GetWindowSize(HVPWindow window) override;
		Point2i GetWindowClientSize(HVPWindow window) override;
		Bounds2i GetWindowBounds(HVPWindow window) override;
		Bounds2i GetWindowClientBounds(HVPWindow window) override;
		Bool8 IsWindowFocus(HVPWindow window) override;
		Bool8 IsMouseInsideWindow(HVPWindow window) override;
		Bool8 IsMouseInsideWindowClient(HVPWindow window) override;


		void PollConnections() override;
		void PollInput() override;
		void PollEvents() override;
		*/
	};
}