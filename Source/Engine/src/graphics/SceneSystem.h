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
		String			mName;
		EntityWorld*	mpWorld;
		Entity			mCamera;

	public:
		Scene(const String& name, EntityWorld* pWorld, Entity camera);

		void SetCamera(Entity entity);

		FORCE_INLINE const String&	GetName() const { return mName; }
		FORCE_INLINE EntityWorld&	GetWorld() { return *mpWorld; }
		FORCE_INLINE Entity			GetCamera() const { return mCamera; }
	};

	class QUARTZ_API SceneManager : public Module
	{
	private:
		Array<Scene*> mScenes;

	public:
		SceneManager();

		Scene* CreateScene(const String& name, EntityWorld* pWorld, Entity camera);
	};
}