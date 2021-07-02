#pragma once

#include "../Module.h"

namespace Quartz
{
	class Application;
	class Window;
	class Scene;

	class QUARTZ_API Game : public Module
	{
	private:
		Application*	pGameApp;
		Window*			pGameWindow;
		Scene*			pGameScene;

	public:
		Game();

		Bool8 PostInit() override;
		void Update(Float32 delta) override;
		void Tick(UInt32 ticks) override;
		void Shutdown() override;
	};
}