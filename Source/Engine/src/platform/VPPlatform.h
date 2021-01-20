#pragma once

#include "Common.h"
#include "util/Array.h"
#include "util/String.h"
#include "math/Bounds.h"

#include "VPDebugConsole.h"

namespace Quartz
{
	/* Virtual Platform Objects */

	struct VPResource {};

	struct VPWindow : public VPResource {};
	struct VPInputMouse : public VPResource {};
	struct VPInputKeyboard : public VPResource {};
	struct VPInputController : public VPResource {};

	/* Virtual Platform Handles */

	typedef VPResource* HVPResource;
	typedef VPResource* HVPWindow;
	typedef VPResource* HVPInputMouse;
	typedef VPResource* HVPInputKeyboard;
	typedef VPResource* HVPInputController;

	/* Null Handles */

#define VP_NULL_HANDLE nullptr

	/* Enums */

	enum ButtonState : UInt8
	{
		BUTTON_STATE_UP = 0x0,
		BUTTON_STATE_DOWN = 0x2
	};

	typedef Flags8 ButtonStates;

	/* Callbacks */

	typedef void (*VPWindowMovedCallback)(HVPWindow window, UInt32 x, UInt32 y);
	typedef void (*VPWindowResizedCallback)(HVPWindow window, UInt32 width, UInt32 height);
	typedef void (*VPWindowFocusCallback)(HVPWindow window, Bool8 focus);
	typedef void (*VPWindowMouseEnteredCallback)(HVPWindow window, Bool8 entered);

	typedef void (*VPMouseMoveCallbackFunc)(HVPInputMouse mouse, Int64 relX, Int64 relY);
	typedef void (*VPMouseButtonCallbackFunc)(HVPInputMouse mouse, UInt32 button, ButtonState state);
	typedef void (*VPKeyboardKeyCallbackFunc)(HVPInputKeyboard keyboard, UInt32 scancode, ButtonState state);

	class QUARTZ_API VPPlatform
	{
	public:
		virtual void PreInitialize() = 0;
		virtual void Initialize() = 0;

		virtual VPDebugConsole* CreateDebugConsole() = 0;
		virtual void DestroyDebugConsole(VPDebugConsole* pDebugConsole) = 0;

		virtual HVPWindow CreateWindow(UInt32 posX, UInt32 posY,
			UInt32 clientWidth, UInt32 clientHeight, const StringW& title) = 0;
		virtual void DestroyWindow(HVPWindow window) = 0;

		virtual void MoveWindow(HVPWindow window, UInt32 x, UInt32 y) = 0;
		virtual void ResizeWindow(HVPWindow window, UInt32 width, UInt32 height) = 0;
		virtual void FocusWindow(HVPWindow window) = 0;
		virtual void ShowWindow(HVPWindow window, Bool8 show) = 0;

		virtual void SetWindowMovedCallback(VPWindowMovedCallback callback) = 0;
		virtual void SetWindowResizedCallback(VPWindowResizedCallback callback) = 0;
		virtual void SetWindowFocusCallback(VPWindowFocusCallback callback) = 0;
		virtual void SetWindowMouseEnteredCallback(VPWindowMouseEnteredCallback callback) = 0;

		virtual void SetMouseMoveCallback(VPMouseMoveCallbackFunc callback) = 0;
		virtual void SetMouseButtonCallback(VPMouseButtonCallbackFunc callback) = 0;
		virtual void SetKeyboardKeyCallback(VPKeyboardKeyCallbackFunc callback) = 0;

		virtual void ShowCursor(Bool8 shown) = 0;
		virtual void CaptureCursor(HVPWindow window) = 0;
		virtual void ReleaseCursor() = 0;

		virtual HVPWindow GetCapturingWindow() = 0;

		virtual Point2i GetCursorPosition() = 0;

		virtual Point2i GetWindowPosition(HVPWindow window) = 0;
		virtual Point2i GetWindowClientPosition(HVPWindow window) = 0;
		virtual Point2i GetWindowSize(HVPWindow window) = 0;
		virtual Point2i GetWindowClientSize(HVPWindow window) = 0;
		virtual Bounds2i GetWindowBounds(HVPWindow window) = 0;
		virtual Bounds2i GetWindowClientBounds(HVPWindow window) = 0;
		virtual Bool8 IsWindowFocus(HVPWindow window) = 0;
		virtual Bool8 IsMouseInsideWindow(HVPWindow window) = 0;
		virtual Bool8 IsMouseInsideWindowClient(HVPWindow window) = 0;

		virtual void PollConnections() = 0;
		virtual void PollInput() = 0;

		virtual void PollEvents() = 0;
	};

	template<>
	FORCE_INLINE UInt32 Hash<VPResource*>(const HVPResource& value)
	{
		return Hash<UInt64>(reinterpret_cast<UInt64>(value));
	}
}