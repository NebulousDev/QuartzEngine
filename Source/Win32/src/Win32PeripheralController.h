#pragma once

#include "Win32.h"
#include <hidsdi.h>

#include "platform/PeripheralController.h"
#include "util/Array.h"
#include "util/Map.h"

namespace Quartz
{
	struct Win32Peripheral : public Peripheral
	{
		HANDLE					deviceHandle;
		Array<HIDP_BUTTON_CAPS> buttonCapsBuffer;
		PHIDP_BUTTON_CAPS		pButtonCaps;
		Array<HIDP_VALUE_CAPS>	valueCapsBuffer;
		PHIDP_VALUE_CAPS		pValueCaps;
	};


	class Win32PeripheralController : public PeripheralController
	{
		typedef Handle64 DeviceHandle;

	private:
		Map<DeviceHandle, Peripheral*> mPeripheralMap;

	public:
		Bool8 RegisterInputUsage(UInt16 usagePage, UInt16 usage, DWORD flags);

		void RescanPeripherals() override;

		void PollInput() override;

		Bool8 IsConnected(Peripheral* pPeripheral) override;
	};
}