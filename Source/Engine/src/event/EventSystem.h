#pragma once

#include "../Module.h"
#include "util\Array.h"
#include "util\Map.h"
#include "Event.h"
#include "EventBuffer.h"
#include "EventDispatcher.h"

#include <iostream>

namespace Quartz
{

#define EVENT_PRIORITY_LOW				0
#define EVENT_PRIORITY_MEDIUM			512
#define EVENT_PRIORITY_HIGH				1024
#define EVENT_PRIORITY_IMMEDIATE		(UInt32)-1

#define SUBSCTIPTION_PRIORITY_LOW		0
#define SUBSCTIPTION_PRIORITY_MEDIUM	512
#define SUBSCTIPTION_PRIORITY_HIGH		1024

	class QUARTZ_API EventSystem : public Module
	{
	private:
		struct EventBucket
		{
			UInt32 priority;
			const EventBase* pEvent;
		};

	private:
		Map<EventTypeId, EventDispatcherBase*>	mDispatchers;
		Map<EventTypeId, EventBufferBase*>		mEventBuffers;
		Array<EventBufferBase*>					mEventBufferList;
		Array<EventBucket>						mEventQueue;

	public:
		EventSystem();

		Bool8 Init() override;

		void Update(Float32 delta) override;

		template<typename EventType>
		void Publish(UInt32 priority, const EventType& event)
		{
			const EventTypeId typeId = EventType::GetStaticEventTypeId();
			EventDispatcherBase** ppDispatcherBase = mDispatchers.Get(typeId);

			if (ppDispatcherBase == nullptr)
			{
				// No dispatchers are subscribed to this event
				return;
			}

			if (priority == EVENT_PRIORITY_IMMEDIATE)
			{
				EventDispatcher<EventType>* pDispatcher = static_cast<EventDispatcher<EventType>*>(*ppDispatcherBase);
				pDispatcher->Dispatch(event);
			}
			else
			{
				EventBufferBase** ppEventBufferBase = mEventBuffers.Get(typeId);
				EventBufferBase* pEventBufferBase = nullptr;

				if (ppEventBufferBase == nullptr)
				{
					pEventBufferBase = new EventBuffer<EventType>(); //TODO: delete pEventBufferBase!!
					mEventBuffers.Put(typeId, pEventBufferBase);
					mEventBufferList.PushBack(pEventBufferBase);
				}
				else
				{
					pEventBufferBase = *ppEventBufferBase;
				}

				EventBuffer<EventType>* pEventBuffer = static_cast<EventBuffer<EventType>*>(pEventBufferBase);
				const EventBase* pStoredEvent = static_cast<const EventBase*>(pEventBuffer->Store(event));

				mEventQueue.PushBack(EventBucket{ priority, pStoredEvent });
			}
		}

		template<typename EventType, typename Scope /* Implicit */>
		void Subscribe(UInt32 priority, Scope* pInstance, EventDispatchFunc<EventType, Scope> dispatchFunc)
		{
			const EventTypeId typeId = EventType::GetStaticEventTypeId();
			EventDispatcherBase** ppDispatcherBase = mDispatchers.Get(typeId);
			EventDispatcherBase* pDispatcherBase = nullptr;

			if (ppDispatcherBase == nullptr)
			{
				pDispatcherBase = new EventDispatcher<EventType>();	//TODO: delete pDispatcherBase!!
				mDispatchers.Put(typeId, pDispatcherBase);
			}
			else
			{
				pDispatcherBase = *ppDispatcherBase;
			}

			EventDispatcher<EventType>* pDispatcher = static_cast<EventDispatcher<EventType>*>(pDispatcherBase);
			pDispatcher->Subscribe(priority, pInstance, dispatchFunc);
		}

		void DispatchEvents();
	};
}