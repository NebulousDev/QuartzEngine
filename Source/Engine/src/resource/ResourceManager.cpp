#include "ResourceManager.h"

#include "util/StringParser.h"

namespace Quartz
{
	String ResourceManager::GetExtension(const String& path)
	{
		StringParser parser(path);
		parser.ReadToChar('.');
		parser.AdvanceChar();
		SubString extension = parser.ReadLine();
		return String(extension);
	}

	void ResourceManager::SetFileSystem(FileSystem* pFileSystem)
	{
		mpFileSystem = pFileSystem;
	}

	void ResourceManager::MarkForUnload(ResourceBase* pResource, Bool8 notify)
	{
		/*
		mResources.Remove(pResource->mUUID);

		if (!pResource->IsVirtual())
		{
			mPhysicalResources.Remove(pResource->mpPathName);
			return;
		}

		delete pResource->mpResourceReference;

		pResource->mpResourceReference->loader->Unload(this, pResource, notify);
		*/
	}

}

