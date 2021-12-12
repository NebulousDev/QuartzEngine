#include "ResourceCashe.h"

namespace Quartz
{
	UUID ResourceCashe::CasheResource(const String& path)
	{
		UUID* pUUID = mPathCashe.Get(path);

		if (pUUID)
		{
			return *pUUID;
		}

		ResourceData data;
		data.path = path;
		data.pResource = nullptr;

		UUID uuid;

		mUUIDCashe.Put(uuid, data);
		mPathCashe.Put(path, uuid);

		return uuid;
	}

	void ResourceCashe::UnCacheResource(const UUID& uuid)
	{
		ResourceData* pResource = mUUIDCashe.Get(uuid);

		if (!pResource)
		{
			// No cached resource
			return;
		}

		mPathCashe.Remove(pResource->path);
		mUUIDCashe.Remove(uuid);

		delete pResource;
	}

	ResourceBase* ResourceCashe::GetLoadedResource(const UUID& uuid)
	{
		ResourceData* pResource = mUUIDCashe.Get(uuid);

		if (!pResource)
		{
			// No cached resource
			return nullptr;
		}

		return pResource->pResource;
	}

	UUID ResourceCashe::GetUUIDFromPath(const String& path)
	{
		UUID* pUUID = mPathCashe.Get(path);

		if (!pUUID)
		{
			return INVALID_UUID;
		}

		return *pUUID;
	}

	Bool8 ResourceCashe::GetPathFromUUID(const UUID& uuid, String& path)
	{
		ResourceData* pData = mUUIDCashe.Get(uuid);

		if (!pData)
		{
			return false;
		}

		path = pData->path;

		return true;
	}

	Bool8 ResourceCashe::IsCached(const UUID& uuid)
	{
		return mUUIDCashe.Contains(uuid);
	}

	Bool8 ResourceCashe::IsLoaded(const UUID& uuid)
	{
		ResourceData* pResource = mUUIDCashe.Get(uuid);
		return pResource && pResource->pResource != nullptr;
	}

	Bool8 ResourceCashe::SetResourceLoaded(const UUID& uuid, ResourceBase* pResource)
	{
		ResourceData* pData = mUUIDCashe.Get(uuid);

		if (!pData)
		{
			return false;
		}

		pData->pResource = pResource;

		return true;
	}

	Bool8 ResourceCashe::SetResourceUnloaded(const UUID& uuid)
	{
		ResourceData* pData = mUUIDCashe.Get(uuid);

		if (!pData)
		{
			return false;
		}

		pData->pResource = nullptr;

		return true;
	}
}

