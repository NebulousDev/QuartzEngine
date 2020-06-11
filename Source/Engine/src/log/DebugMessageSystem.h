#pragma once

#include "../system/System.h"
#include "PlatformConsole.h"

#include "DebugMessageEvent.h"

namespace Quartz
{
	class QUARTZ_API DebugMessageSystem : public System
	{
	private:
		PlatformConsole* mpDebugConsole;

		Bool8 ParseDebugMessageEvent(const DebugMessageEvent& event);

	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}