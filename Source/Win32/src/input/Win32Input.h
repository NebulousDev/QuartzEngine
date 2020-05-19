#pragma once

#include "input\Input.h"

#include "../Win32.h"
#include <hidsdi.h>

namespace Quartz
{
	class QUARTZ_API Win32Input : public Input
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
		Map<Handle64, InputDevice>		mHandleToDevice;

	public:
		void InitializeInput() override;
		void PollDeviceConnections() override;

		Bool8 CreateDevice(InputDeviceDescription deviceDesc, Flags32 flags, InputDevice* pDevice) override;
		void DestroyDevice(InputDevice device) override;

		void PollDeviceInputs() override;
	};
}