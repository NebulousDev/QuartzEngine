#pragma once

#include "../entity/System.h"
#include "PlatformConsole.h"

#include "DebugMessageEvent.h"

namespace Quartz
{
	class QUARTZ_API DebugMessageSystem : public SingletonSystem
	{
	private:
		PlatformConsole* mpDebugConsole;

		Bool8 ParseDebugMessageEvent(const DebugMessageEvent& event);

	public:
		void OnInit(EntityWorld& world) override;
		void OnUpdate(EntityWorld& world, Float32 deltaTime) override;
		void OnTick(EntityWorld& world, Float32 deltaTime) override;
		void OnDestroy(EntityWorld& world) override;
	};
}