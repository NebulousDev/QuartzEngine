#pragma once

#include "../ResourceManager.h"

#include "JSONLoader.h"
#include "ImageLoader.h"
#include "OBJLoader.h"
#include "GLTFLoader.h"
#include "SpirvLoader.h"
#include "ShaderLoader.h"
#include "MaterialLoader.h"

namespace Quartz
{
	namespace Loaders
	{
		Bool8 LoadJSON(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, JSON** pJson);
		void UnloadJSON(ResourceManager* pResourceManager, FileSystem* pFileSystem, JSON* pJson);

		Bool8 LoadRawImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, RawImage** ppRawImage);
		void UnloadRawImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, RawImage* pRawImage);

		Bool8 LoadOBJModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, OBJModel** ppObjModel);
		void UnloadOBJModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, OBJModel* pObjModel);

		Bool8 LoadGLTFModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, GLTFModel** ppGltfModel);
		void UnloadGLTFModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, GLTFModel* pGltfModel);

		Bool8 LoadSpirvObject(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, SpirvObject** ppSpirvObject);
		void UnloadSpirvObject(ResourceManager* pResourceManager, FileSystem* pFileSystem, SpirvObject* pSpirvObject);

		Bool8 LoadShader(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Shader** ppShader);
		void UnloadShader(ResourceManager* pResourceManager, FileSystem* pFileSystem, Shader* pShader);

		Bool8 LoadMaterial(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Material** ppMaterial);
		void UnloadMaterial(ResourceManager* pResourceManager, FileSystem* pFileSystem, Material* pMaterial);

		void RegisterCoreLoaders(ResourceManager* pResourceManager);
	}
}