#pragma once

#include "../Module.h"
#include "../entity/World.h"

#include "util/Array.h"

namespace Quartz
{
	// @TODO: Split into GameScene and UIScene
	class QUARTZ_API Scene
	{
	private:
		String		mName;
		EntityWorld mWorld;
		Entity		mCamera;

	public:
		Scene(const String& name);

		void SetCamera(Entity entity);

		FORCE_INLINE const String&	GetName() const { return mName; }
		FORCE_INLINE EntityWorld&	GetWorld() { return mWorld; }
		FORCE_INLINE Entity			GetCamera() { return mCamera; }
	};

	class QUARTZ_API SceneManager : public Module
	{
	private:
		Array<Scene*> mScenes;

	public:
		SceneManager();

		Scene* CreateScene(const String& name);
	};
}