#include "ResourceManager.h"

#include "util/StringParser.h"

namespace Quartz
{
	AssetBase::AssetBase(AssetReference* pAssetReference) :
		mpAssetReference(pAssetReference)
	{
		if (mpAssetReference != nullptr)
		{
			mpAssetReference->count++;
		}
	}

	AssetBase::~AssetBase()
	{
		if (mpAssetReference != nullptr)
		{
			mpAssetReference->count--;

			if (mpAssetReference->count == 0)
			{
				AssetManager::GetInstance().MarkForUnload(mpAssetReference);
			}
		}
	}

	Bool8 AssetBase::IsValid() const
	{
		return mpAssetReference != nullptr;
	}

	UInt32 AssetBase::Count() const
	{
		if (mpAssetReference != nullptr)
		{
			return mpAssetReference->count;
		}
		
		return 0;
	}

	String AssetManager::GetExtension(const String& path)
	{
		StringParser parser(path);
		parser.ReadToChar('.');
		parser.AdvanceChar();
		SubString extension = parser.ReadLine();
		return String(extension);
	}

	void AssetManager::MarkForUnload(AssetReference* pAssetReference)
	{
		mMarkedForUnload.Push(pAssetReference);
	}

	void AssetManager::SetAssetRoot(const String& root)
	{
		mRoot = root;
	}
}

