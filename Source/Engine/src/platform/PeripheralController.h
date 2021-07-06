#pragma once

#include "Peripheral.h"

namespace Quartz
{
	class QUARTZ_API PeripheralController
	{
	public:
		virtual void	RescanPeripherals() = 0;
		virtual void	PollInput() = 0;
		virtual Bool8	IsConnected(Peripheral* pPeripheral) = 0;
	};
}