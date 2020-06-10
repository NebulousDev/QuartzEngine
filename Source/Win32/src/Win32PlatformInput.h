#pragma once

#include "PlatformInput.h"

#include "Win32.h"
#include <hidsdi.h>

namespace Quartz
{
	class QUARTZ_API Win32PlatformInput : public PlatformInput
	{
	private:
		struct Win32InputDeviceDescExt
		{
			HANDLE					deviceHandle;
			Array<HIDP_BUTTON_CAPS> buttonCapsBuffer;
			PHIDP_BUTTON_CAPS		pButtonCaps;
			Array<HIDP_VALUE_CAPS>	valueCapsBuffer;
			PHIDP_VALUE_CAPS		pValueCaps;
		};

		Array<Win32InputDeviceDescExt>	mInputDeviceDescExts;
		Map<Handle64, InputDeviceId>	mHandleToDevice;

		//Bool8 CreateDevice(InputDeviceDesc deviceDesc, Flags32 flags);
		//void DestroyDevice(InputDeviceId deviceId);

	public:
		void PollDeviceConnections() override;
		void PollDeviceInput() override;
	};
}