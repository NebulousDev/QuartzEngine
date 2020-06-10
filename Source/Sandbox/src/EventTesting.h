#pragma once

#include "io\Log.h"

#include <iostream>

namespace Quartz
{
	struct TestEvent1 : public Event<TestEvent1>
	{
		UInt32 test;
	};

	struct TestEvent2 : public Event<TestEvent2>
	{
		UInt32 test;
	};

	struct TestEvent3 : public Event<TestEvent3>
	{
		UInt32 test;
	};

	Bool8 Event1Func(const TestEvent1& event)
	{
		Log.Info("Event1Func called | Value: %d", event.test);
		return true;
	}

	Bool8 Event2Func(const TestEvent2& event)
	{
		Log.Info("Event2Func called | Value: %d", event.test);
		return true;
	}

	Bool8 Event3Func(const TestEvent3& event)
	{
		Log.Info("Event3Func called | Value: %d", event.test);
		return true;
	}

	Bool8 Event3Func2(const TestEvent3& event)
	{
		Log.Info("Event3Func2 called | Value: %d", event.test);
		return false;
	}

	Bool8 Event3Func3(const TestEvent3& event)
	{
		Log.Info("Event3Func3 called | Value: %d", event.test);
		return true;
	}

	void TestEvents()
	{
		EventSystem eventSystem;

		TestEvent1 ev1;
		TestEvent2 ev2;
		TestEvent3 ev3;

		Log.Info("EV1: %lu", ev1.GetStaticEventTypeId());
		Log.Info("EV2: %lu", ev2.GetStaticEventTypeId());
		Log.Info("EV3: %lu", ev3.GetStaticEventTypeId());

		eventSystem.Subscribe<TestEvent1>(SUBSCTIPTION_PRIORITY_LOW, Event1Func);
		eventSystem.Subscribe<TestEvent2>(SUBSCTIPTION_PRIORITY_LOW, Event2Func);
		eventSystem.Subscribe<TestEvent3>(SUBSCTIPTION_PRIORITY_LOW, Event3Func);
		eventSystem.Subscribe<TestEvent3>(SUBSCTIPTION_PRIORITY_LOW, Event3Func2);
		eventSystem.Subscribe<TestEvent3>(SUBSCTIPTION_PRIORITY_LOW, Event3Func3);

		ev1.test = 25;
		ev2.test = 26;
		ev3.test = 27;
		
		eventSystem.Publish(EVENT_PRIORITY_IMMEDIATE, ev1);
		eventSystem.Publish(EVENT_PRIORITY_LOW, ev3);
		eventSystem.Publish(EVENT_PRIORITY_LOW, ev2);

		eventSystem.DispatchEvents();

		system("PAUSE");
	}
}