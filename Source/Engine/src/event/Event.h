#pragma once

#include "Common.h"
#include "util\TypeId.h"

namespace Quartz
{
	typedef TypeId EventTypeId;

	class EventBase
	{
	public:
		virtual EventTypeId GetEventTypeId() const = 0;
	};

	template<typename EventType>
	class Event : public EventBase
	{
	private:
		EventTypeId GetEventTypeId() const override;

	public:
		static EventTypeId GetStaticEventTypeId();
	};

	template<typename EventType>
	EventTypeId Event<EventType>::GetEventTypeId() const
	{
		return GetStaticEventTypeId();
	}

	template<typename EventType>
	EventTypeId Event<EventType>::GetStaticEventTypeId()
	{
		return TypeInfo<EventType>::Id();
	}
}