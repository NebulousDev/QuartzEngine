#include "SceneSystem.h"

#include "../Engine.h"

namespace Quartz
{
	Scene::Scene(const String& name, EntityWorld* pWorld, Entity camera)
		: mName(name),
		mpWorld(pWorld),
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

	Scene* SceneManager::CreateScene(const String& name, EntityWorld* pWorld, Entity camera)
	{
		Scene* pScene = new Scene(name, pWorld, camera);
		mScenes.PushBack(pScene);
		
		return pScene;
	}
}

