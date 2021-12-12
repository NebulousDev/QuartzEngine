#pragma once

#include "../Image.h"
#include "../object/Model.h"

#include "../../resource/ResourceManager.h"

namespace Quartz
{
	namespace Loaders
	{
		Bool8 LoadImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Image** ppImage);
		void UnloadImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, Image* pImage);

		Bool8 LoadModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Model** ppModel);
		void UnloadModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, Model* pModel);

		void RegisterGraphicsLoaders(ResourceManager* pResourceManager);
	}
}