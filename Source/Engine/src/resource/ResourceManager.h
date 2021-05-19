#pragma once

#include "Common.h"
#include "util/Singleton.h"
#include "util/String.h"
#include "util/Map.h"
#include "util/Stack.h"

#include "../log/Log.h"

namespace Quartz
{
	struct AssetReference
	{
		UInt32 count = 0;
		void* pAsset = nullptr;
	};

	class QUARTZ_API AssetBase
	{
	protected:
		AssetReference* mpAssetReference;

	public:
		AssetBase(AssetReference* pAssetReference);
		~AssetBase();

		Bool8 IsValid() const;
		UInt32 Count() const;
	};

	template<typename AssetType>
	class Asset : public AssetBase
	{
	public:
		Asset(AssetReference* pAssetReference) :
			AssetBase(pAssetReference) {}

		AssetType* GetAsset()
		{
			return static_cast<AssetType*>(mpAssetReference->pAsset);
		}
	};

	template<typename AssetType>
	using LoadAssetFunc = AssetType* (*)(const String& path);

	template<typename AssetType>
	using UnloadAssetFunc = Bool8 (*)(AssetType* pAsset);

	struct ExtLoaderBase { };

	template<typename AssetType>
	class ExtLoader : public ExtLoaderBase
	{
	private:
		LoadAssetFunc<AssetType> mLoadFunc;
		UnloadAssetFunc<AssetType> mUnloadFunc;

	public:
		ExtLoader(LoadAssetFunc<AssetType> loadFunc,
			UnloadAssetFunc<AssetType> unloadFunc) :
			mLoadFunc(loadFunc),
			mUnloadFunc(unloadFunc) { }

		AssetType* Load(const String& path)
		{
			return mLoadFunc(path);
		}

		Bool8 Unload(AssetType* pAsset)
		{
			return mUnloadFunc(pAsset);
		}
	};

	class QUARTZ_API AssetManager : public Singleton<AssetManager>
	{
	public:
		friend class AssetBase;

	private:
		String mRoot;
		Map<String, ExtLoaderBase*> mExtLoaders;
		Map<String, AssetReference> mAssetReferences;
		Stack<AssetReference*> mMarkedForUnload;

		String GetExtension(const String& path);

		void MarkForUnload(AssetReference* pAssetReference);

	public:
		void SetAssetRoot(const String& root);

		template<typename AssetType>
		void RegisterAssetLoaders(const String& ext, LoadAssetFunc<AssetType> loadFunc, UnloadAssetFunc<AssetType> unloadFunc)
		{
			ExtLoaderBase*& pLoaderBase = mExtLoaders[ext];

			if (pLoaderBase != nullptr)
			{
				Log::Warning("Attempted to register new asset loaders for the existing \
				'%s' extension. Overwriting loaders...", ext.Str());

				delete pLoaderBase;
			}

			pLoaderBase = new ExtLoader<AssetType>(loadFunc, unloadFunc);
		}

		template<typename AssetType>
		Asset<AssetType> GetAsset(const String& path)
		{
			AssetReference& reference = mAssetReferences[path];

			if (reference.count > 0)
			{
				return Asset<AssetType>(&reference);
			}
			else
			{
				String ext = GetExtension(path);
				ExtLoader<AssetType>* pLoader = static_cast<ExtLoader<AssetType>*>(mExtLoaders[ext]);

				if (pLoader != nullptr)
				{
					Log::General("Loading asset ['%s']", path.Str());

					AssetType* pAsset = pLoader->Load(path);

					reference.count = 0;
					reference.pAsset = pAsset;

					if (pAsset == nullptr)
					{
						Log::Error("Error loading asset ['%s']: Loader failed!", path.Str());
					}

					return Asset<AssetType>(&reference);
				}
				else
				{
					Log::Error("Error loading asset ['%s']: \
						No loaders registered for extension '%s'!", path.Str(), ext.Str());
				}

				return Asset<AssetType>(nullptr);
			}
		}
	};
}