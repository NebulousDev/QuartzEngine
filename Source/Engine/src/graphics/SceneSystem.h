#pragma once

#include "../Module.h"
#include "../physics/SceneGraph.h"

#include "util/Array.h"

namespace Quartz
{
	// @TODO: Split into GameScene and UIScene
	class QUARTZ_API Scene
	{
	private:
		String			mName;
		EntityWorld*	mpWorld;
		SceneGraphView	mGraph;
		Entity			mCamera;

	public:
		Scene(const String& name, EntityWorld* pWorld, const SceneGraphView& graph, Entity camera);

		void SetCamera(Entity entity);

		FORCE_INLINE const String&		GetName() const { return mName; }
		FORCE_INLINE EntityWorld&		GetWorld() { return *mpWorld; }
		FORCE_INLINE SceneGraphView		GetGraph() const { return mGraph; }
		FORCE_INLINE Entity				GetCamera() const { return mCamera; }
	};

	class QUARTZ_API SceneManager : public Module
	{
	private:
		Array<Scene*> mScenes;

	public:
		SceneManager();

		Scene* CreateScene(const String& name, const SceneGraphView& graph, Entity camera);
	};
}