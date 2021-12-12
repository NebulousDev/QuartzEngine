#pragma once

#include "../Module.h"
#include "../platform/Application.h"

#include "../input/InputEvents.h"
#include "../platform/PeripheralEvent.h"

#include "../entity/Entity.h"

namespace Quartz
{
	class Application;
	class Window;
	class Scene;
	class Surface;
	class Renderer;
	class Context;

	class QUARTZ_API Game : public Module
	{
	private:
		Application*	mpGameApp;
		Window*			mpGameWindow;
		Scene*			mpGameScene;
		Surface*		mpSurface;
		Renderer*		mpRenderer;
		Context*		mpGameViewport;

		Entity mCamera;
		Entity mEntity1;
		Entity mEntity2;
		Entity mLight;

		Bool8 OnWindowClose(const WindowCloseEvent& event);

		Bool8 OnPeripheralConnection(const PeripheralEvent& event);

	public:
		Game();

		Bool8 PostInit() override;
		void Update(Float32 delta) override;
		void Tick(UInt32 ticks) override;
		void Shutdown() override;
	};
}