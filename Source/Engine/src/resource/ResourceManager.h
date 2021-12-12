#pragma once

#include "../platform/FileSystem.h"
#include "Resource.h"
#include "ResourceCashe.h"
#include "util/Map.h"
#include "util/Stack.h"
#include "util/TypeId.h"
#include "util/UUID.h"

#include "../log/Log.h"

namespace Quartz
{
	class ResourceManager;

	template<typename ResourceType>
	using LoadResourceFunc = Bool8(*)(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, ResourceType** ppResource);

	template<typename ResourceType>
	using UnloadResourceFunc = void (*)(ResourceManager* pResourceManager, FileSystem* pFileSystem, ResourceType* pResource);

	struct QUARTZ_API ResourceLoaderBase
	{
	public:
		virtual Bool8 Load(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, void** ppResource, Bool8 notify) = 0;
		virtual void Unload(ResourceManager* pResourceManager, FileSystem* pFileSystem, void* pResource, Bool8 notify) = 0;
	};

	template<typename ResourceType>
	class QUARTZ_API ResourceLoader : public ResourceLoaderBase
	{
	private:
		LoadResourceFunc<ResourceType>		mLoadFunc;
		UnloadResourceFunc<ResourceType>	mUnloadFunc;

	public:
		ResourceLoader(LoadResourceFunc<ResourceType> loadFunc,
			UnloadResourceFunc<ResourceType> unloadFunc) :
			mLoadFunc(loadFunc),
			mUnloadFunc(unloadFunc) { }

		Bool8 Load(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, void** ppResource, Bool8 notify) override
		{
			if (notify)
			{
				Log::General("Loading resource [0x%08X]['%s']", TypeId<ResourceType>::Value(), path.Str());
			}

			String finalPath = path;

			if (pFileSystem->IsVirtualPath(path))
			{
				pFileSystem->ResolvePhysicalPath(path, finalPath);
			}

			return mLoadFunc(pResourceManager, pFileSystem, finalPath, (ResourceType**)(ppResource));
		}

		void Unload(ResourceManager* pResourceManager, FileSystem* pFileSystem, void* pResource, Bool8 notify) override
		{
			ResourceBase* pResourceBase = static_cast<ResourceBase*>(pResource);

			if (notify)
			{
				Log::General("Unloading resource [0x%08X]['%s']", TypeId<ResourceType>::Value(), pResourceBase->GetPathName().Str());
			}

			mUnloadFunc(pResourceManager, pFileSystem, static_cast<ResourceType*>(pResourceBase));
		}
	};

	class QUARTZ_API ResourceManager
	{
	public:
		friend class ResourceBase;

		typedef UInt64 LoaderId;

	private:
		FileSystem*							mpFileSystem;
		ResourceCashe						mResourceCashe;
		//String								mRelativePath;
		Map<LoaderId, ResourceLoaderBase*>	mResourceLoaders;
		//Map<UUID, ResourceBase*>			mResources;
		//Map<String, UUID>					mPhysicalResources;
		Stack<ResourceBase*>				mMarkedForUnload;

	private:
		template<typename ResourceType>
		Bool8 LoadResource(UUID uuid, ResourceBase** ppResource, Bool8 notify)
		{
			String path;
			mResourceCashe.GetPathFromUUID(uuid, path);

			LoaderId loaderId = TypeId<ResourceType>::Value();
			ResourceLoaderBase** ppLoaderBase = mResourceLoaders.Get(loaderId);

			if (!ppLoaderBase)
			{
				Log::Error("Error loading resource ['%s']: No loader for type [0x%08X] is registered!", path.Str(), loaderId);

				return false;
			}

			ResourceLoader<ResourceType>* pLoader = static_cast<ResourceLoader<ResourceType>*>(*ppLoaderBase);

			if (!pLoader)
			{
				Log::Error("Error loading resource ['%s']: Loader for type [0x%08X] returned null!", path.Str(), loaderId);

				return false;
			}

			Bool8 result = pLoader->Load(this, mpFileSystem, path, (void**)ppResource, notify);

			if (!result)
			{
				Log::Error("Error loading resource ['%s']: Loader failed!", path.Str());
				return false;
			}

			ResourceBase* pResource = *ppResource;

			pResource->mUUID = uuid;
			pResource->mpPathName = path;
			pResource->mpResourceManager = this;
			pResource->mpResourceReference = new ResourceReference{ pLoader, 1 };

			mResourceCashe.SetResourceLoaded(uuid, pResource);

			return true;
		}

		void MarkForUnload(ResourceBase* pResource, Bool8 notify);

	public:
		// TODO: Move to util class?
		String GetExtension(const String& path);

		void SetFileSystem(FileSystem* pFileSystem);

		template<typename ResourceType>
		void RegisterResourceLoader(LoadResourceFunc<ResourceType> loadFunc, UnloadResourceFunc<ResourceType> unloadFunc)
		{
			LoaderId loaderId = TypeId<ResourceType>::Value();
			ResourceLoaderBase** ppLoaderBase = mResourceLoaders.Get(loaderId);

			if (ppLoaderBase != nullptr)
			{
				Log::Warning("Attempted to register new resource loaders for the existing type [0x%08X]. Overwriting loaders...", loaderId);

				delete *ppLoaderBase;
			}

			// TODO: new can be avoided with clever casts
			mResourceLoaders.Put(loaderId, new ResourceLoader<ResourceType>(loadFunc, unloadFunc));
		}

		template<typename ResourceType>
		UUID FindResource(const String& path)
		{
			if (!mpFileSystem->IsValidPath(path))
			{
				return INVALID_UUID;
			}

			UUID uuid = mResourceCashe.CasheResource(path);

			return uuid;
		}

		/*
		template<typename ResourceType>
		UInt64 AddVirtualResource(ResourceType resource)
		{
			ResourceType* pResource = new ResourceType(resource);

			pResource->mUUID = UUID();
			pResource->mpPathName = String();
			pResource->mpResourceManager = this;
			pResource->mpResourceReference = new ResourceReference{ nullptr, 1 };

			mResources.Put(pResource->mUUID, pResource);
		}
		*/

		template<typename ResourceType>
		ResourceType* CheckoutResource(UUID uuid, Bool8 notify = true)
		{
			if (mResourceCashe.IsLoaded(uuid))
			{
				ResourceBase* pResourceBase = mResourceCashe.GetLoadedResource(uuid);
				pResourceBase->IncrementReference();
				return static_cast<ResourceType*>(pResourceBase);
			}
			else
			{
				ResourceBase* pResourceBase = nullptr;
				if (!LoadResource<ResourceType>(uuid, &pResourceBase, notify))
				{
					return nullptr;
				}

				return static_cast<ResourceType*>(pResourceBase);
			}
		}

		template<typename ResourceType>
		ResourceType* CheckoutResource(const String& path, Bool8 notify = true)
		{
			if (!mpFileSystem->IsValidPath(path))
			{
				return nullptr;
			}

			UUID uuid = mResourceCashe.CasheResource(path);

			return CheckoutResource<ResourceType>(uuid, notify);
		}

		void ReturnResource(ResourceBase* pResource, Bool8 notify = true)
		{
			if (pResource)
			{
				pResource->DecrementReference();

				if (pResource->ReferenceCount() == 0)
				{
					MarkForUnload(pResource, notify);
				}
			}
		}
	};
}