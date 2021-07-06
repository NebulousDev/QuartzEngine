#pragma once

#include "Common.h"
#include "util\Array.h"

namespace Quartz
{
	class EventBufferBase
	{
	public:
		virtual void		Clear() = 0;
		virtual EventBase*	GetEvent(UInt32 index) = 0;
	};

	template<typename EventType>
	class EventBuffer : public EventBufferBase
	{
	private:
		Array<EventType> mEvents;

	public:
		/** 
			Store an event in the Queue
			Returns an index to the stored event
		*/
		UInt32 Store(const EventType& event)
		{
			mEvents.PushBack(event);
			return mEvents.Size() - 1;
		}

		/** 
			Clears the event buffer
			WARNING: Invalidates all pointers
		*/
		void Clear() override
		{
			mEvents.Clear();
		}

		EventType* GetEvent(UInt32 index) override
		{
			return &mEvents[index];
		}
	};
}