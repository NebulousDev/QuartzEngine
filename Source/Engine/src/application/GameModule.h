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
		void Tick() override;
		void Shutdown() override;
	};
}