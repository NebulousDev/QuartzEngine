#include "EventSystem.h"

namespace Quartz
{
	EventSystem::EventSystem()
		: Module({ L"Event System" })
	{
		// Nothing
	}

	Bool8 EventSystem::Init()
	{
		return true;
	}

	void EventSystem::Update(Float32 delta)
	{
		DispatchEvents();
	}

	void EventSystem::DispatchEvents()
	{
		if (mEventQueue.Size() > 0)
		{
			for (UInt32 i = 0; i < mEventQueue.Size(); i++)
			{
				EventBucket& bucket = mEventQueue[i];
				EventTypeId typeId = bucket.pEvent->GetEventTypeId();

				EventDispatcherBase* pDispatcherBase = mDispatchers[typeId]; // shouldn't be nullptr?
				pDispatcherBase->Dispatch(bucket.pEvent);
			}

			mEventQueue.Clear();

			// Clear Event Buffers:

			for (UInt32 i = 0; i < mEventBufferList.Size(); i++)
			{
				EventBufferBase* pEventBufferBase = mEventBufferList[i];
				pEventBufferBase->Clear();
			}
		}
	}

}

