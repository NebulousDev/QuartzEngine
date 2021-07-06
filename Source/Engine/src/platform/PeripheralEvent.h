#pragma once

#include "../event/Event.h"
#include "../platform/Peripheral.h"

namespace Quartz
{
	enum PeripheralEventType
	{
		PERIPHERAL_CONNECTED,
		PERIPHERAL_DISCONNECTED
	};

	struct PeripheralEvent : public Event<PeripheralEvent>
	{
		Peripheral*			pPeripheral;
		PeripheralEventType	eventType;

		inline ~PeripheralEvent()
		{
			int hello = 0;
		}
	};
}