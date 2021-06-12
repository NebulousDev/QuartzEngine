#include "SceneSystem.h"

namespace Quartz
{
	Scene::Scene(const String& name)
		: mName(name)
	{
		// Nothing
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

