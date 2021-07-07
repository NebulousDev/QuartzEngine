#include "SceneSystem.h"

namespace Quartz
{
	Scene::Scene(const String& name)
		: mName(name)
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

	Scene* SceneManager::CreateScene(const String& name)
	{
		Scene* pScene = new Scene(name);
		mScenes.PushBack(pScene);
		return pScene;
	}
}

