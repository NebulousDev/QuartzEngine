#pragma once

#include "Common.h"
#include "util\Array.h"

namespace Quartz
{
	class EventBufferBase
	{
	public:
		virtual void Clear() = 0;
	};

	template<typename EventType>
	class EventBuffer : public EventBufferBase
	{
	private:
		Array<EventType> mEvents;

	public:
		/** 
			Store an event in the Queue
			Returns a pointer to the stored event
		*/
		const EventType* Store(const EventType& event);

		/** 
			Clears the event buffer
			WARNING: Invalidates all pointers
		*/
		void Clear() override;
	};

	template<typename EventType>
	const EventType* EventBuffer<EventType>::Store(const EventType& event)
	{
		mEvents.PushBack(event);
		return &(mEvents[mEvents.Size() - 1]);
	}

	template<typename EventType>
	void EventBuffer<EventType>::Clear()
	{
		mEvents.Clear();
	}
}