#include "SceneSystem.h"

#include "../Engine.h"

namespace Quartz
{
	Scene::Scene(const String& name, EntityWorld* pWorld, const SceneGraphView& graph, Entity camera)
		: mName(name),
		mpWorld(pWorld),
		mGraph(graph),
		mCamera(camera)
	{
		// Nothing
	}

	void Scene::SetCamera(Entity entity)
	{
		mCamera = entity;
	}

	Quartz::SceneManager::SceneManager()
		: Module({ L"Scene System" })
	{
		// Nothing
	}

	Scene* SceneManager::CreateScene(const String& name, const SceneGraphView& graph, Entity camera)
	{
		EntityWorld* pWorld = Engine::GetInstance()->GetEntityWorld();

		Scene* pScene = new Scene(name, pWorld, graph, camera);
		mScenes.PushBack(pScene);
		
		return pScene;
	}
}

