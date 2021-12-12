#pragma once

#include "Resource.h"
#include "util/Map.h"
#include "util/String.h"
#include "util/UUID.h"

namespace Quartz
{
	class QUARTZ_API ResourceCashe
	{
	private:
		struct ResourceData
		{
			String			path;
			ResourceBase*	pResource;
		};

	private:
		Map<UUID, ResourceData> mUUIDCashe;
		Map<String, UUID>		mPathCashe;

	public:
		UUID CasheResource(const String& path);
		void UnCacheResource(const UUID& uuid);

		ResourceBase* GetLoadedResource(const UUID& uuid);
		Bool8 SetResourceLoaded(const UUID& uuid, ResourceBase* pResource);
		Bool8 SetResourceUnloaded(const UUID& uuid);

		UUID GetUUIDFromPath(const String& path);
		Bool8 GetPathFromUUID(const UUID& uuid, String& path);

		Bool8 IsCached(const UUID& uuid);
		Bool8 IsLoaded(const UUID& uuid);
		
	};
}