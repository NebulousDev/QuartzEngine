#include "DebugMessageSystem.h"

#include "../Engine.h"

#include "Log.h"

namespace Quartz
{

	Bool8 DebugMessageSystem::ParseDebugMessageEvent(const DebugMessageEvent& event)
	{
		switch (event.category)
		{
			case MESSAGE_CRITICAL:
			{
				mpDebugConsole->SetColor(COLOR_WHITE, COLOR_RED);
				break;
			}

			case MESSAGE_ERROR:
			{
				mpDebugConsole->SetColor(COLOR_LIGHT_RED, COLOR_DEFAULT);
				break;
			}

			case MESSAGE_WARNING:
			{
				mpDebugConsole->SetColor(COLOR_YELLOW, COLOR_DEFAULT);
				break;
			}

			case MESSAGE_GENERAL:
			{
				mpDebugConsole->SetColor(COLOR_WHITE, COLOR_DEFAULT);
				break;
			}

			case MESSAGE_INFO:
			{
				mpDebugConsole->SetColor(COLOR_WHITE, COLOR_DEFAULT);
				break;
			}

			case MESSAGE_DEBUG:
			{
				mpDebugConsole->SetColor(COLOR_LIGHT_GRAY, COLOR_DEFAULT);
				break;
			}

			case MESSAGE_NONE:
			{
				mpDebugConsole->SetColor(COLOR_DEFAULT, COLOR_DEFAULT);
				break;
			}

			default: break;
		}

		mpDebugConsole->Print(event.message.Str());
		mpDebugConsole->SetColor(COLOR_DEFAULT, COLOR_DEFAULT);

		return true;
	}

	void DebugMessageSystem::Initialize()
	{
		Engine& engine = Engine::GetInstance();

		mpDebugConsole = engine.GetPlatform().CreateConsole();
		mpDebugConsole->Create();
		mpDebugConsole->Show();

		engine.GetEventSystem().Subscribe(SUBSCTIPTION_PRIORITY_HIGH, this, &DebugMessageSystem::ParseDebugMessageEvent);
	}

	void DebugMessageSystem::Update()
	{
		
	}

	void DebugMessageSystem::Tick()
	{

	}

	void DebugMessageSystem::Destroy()
	{

	}
}

