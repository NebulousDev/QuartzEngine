#pragma once

#include "PlatformInput.h"

#include "Win32.h"
#include <hidsdi.h>

namespace Quartz
{
	class QUARTZ_API Win32PlatformInput : public PlatformInput
	{
	private:
		struct Win32InputDeviceInfo
		{
			HANDLE					deviceHandle;
			Array<HIDP_BUTTON_CAPS> buttonCapsBuffer;
			PHIDP_BUTTON_CAPS		pButtonCaps;
			Array<HIDP_VALUE_CAPS>	valueCapsBuffer;
			PHIDP_VALUE_CAPS		pValueCaps;
		};

		Array<Win32InputDeviceInfo>		mWin32InputDeviceInfos;
		Map<Handle64, InputDeviceId>	mHandleToDevice;

		Array<RAWINPUTDEVICELIST>	mPolledRawInputDevices;
		Set<InputDeviceId>			mPolledIds;

		Window* mpCapturedWindow;

	public:
		void PollConnections() override;
		void PollInput() override;
		Point2i GetMousePosition() const override;
		void SetMousePosition(Point2i position) override;

		void HideCursor() override;
		void ShowCursor() override;
		void SetCursorBounds(Bounds2i bounds) override;
		void ReleaseCursorBounds() override;
	};
}