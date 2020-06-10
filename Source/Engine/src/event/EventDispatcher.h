#pragma once

#include "util\Array.h"
#include "Event.h"

namespace Quartz
{
	template<typename EventType, typename Scope>
	using EventDispatchFunc = Bool8(Scope::*)(const EventType& events);

	class EventDispatcherBase
	{
	public:
		virtual void Dispatch(const EventBase* pEvent) = 0;
	};

	template<typename EventType>
	class EventDispatcher : public EventDispatcherBase
	{
	private:

		class IEventDispatch
		{
		public:
			virtual Bool8 operator() (const EventType& event) = 0;
		};

		template<typename Scope>
		class EventDispatch : public IEventDispatch
		{
		private:
			Scope* mpInstance;
			EventDispatchFunc<EventType, Scope> mDispatchFunc;

		public:
			EventDispatch(Scope* pInstance, EventDispatchFunc<EventType, Scope> dispatch);
			Bool8 operator() (const EventType& event) override;
		};

		struct DispatchBucket
		{
			UInt32			priority;
			IEventDispatch*	pDispatch;
		};

		void Dispatch(const EventBase* pEvent) override;

	private:
		Array<IEventDispatch> mDispatchBuffer;
		Array<DispatchBucket> mDispatchQueue;

	public:
		template<typename Scope>
		void Subscribe(UInt32 priority, Scope* pInstance, EventDispatchFunc<EventType, Scope>& dispatchFunc);

		void Dispatch(const EventType& event);
	};

	template<typename EventType>
	template<typename Scope>
	EventDispatcher<EventType>::EventDispatch<Scope>::
		EventDispatch(Scope* pInstance, EventDispatchFunc<EventType, Scope> dispatch)
	{
		mpInstance = pInstance;
		mDispatchFunc = dispatch;
	}

	template<typename EventType>
	template<typename Scope>
	Bool8 EventDispatcher<EventType>::EventDispatch<Scope>::
		operator() (const EventType& event)
	{
		return (mpInstance->*mDispatchFunc)(event);
	}

	template<typename EventType>
	void EventDispatcher<EventType>::Dispatch(const EventBase* pEvent)
	{
		this->Dispatch(static_cast<const EventType&>(*pEvent));
	}

	template<typename EventType>
	template<typename Scope>
	void EventDispatcher<EventType>::Subscribe(UInt32 priority, Scope* pInstance, EventDispatchFunc<EventType, Scope>& dispatchFunc)
	{
		EventDispatch<Scope>* pEventDispatch = new EventDispatch<Scope>(pInstance, dispatchFunc); // Allocate in buffer
		mDispatchQueue.PushBack(DispatchBucket{ priority, pEventDispatch });
	}

	template<typename EventType>
	void EventDispatcher<EventType>::Dispatch(const EventType& event)
	{
		for (UInt32 i = 0; i < mDispatchQueue.Size(); i++)
		{
			IEventDispatch* pDispatch = mDispatchQueue[i].pDispatch;

			if (!(*pDispatch)(event))
			{
				// dispatch returned false
				// do not send message to any other system
				break;
			}
		}
	}
}


