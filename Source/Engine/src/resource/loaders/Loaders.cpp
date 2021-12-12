#include "Loaders.h"

#include "LoaderUtils.h"
#include "JSONLoader.h"

namespace Quartz
{
    Bool8 Loaders::LoadJSON(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, JSON** ppJson)
    {
        String jsonString;
        Bool8 result;

        result = ReadFileString(path, jsonString);

        if (!result)
        {
            return false;
        }

        JSON* pJson = new JSON();
        result = LoadJSON(jsonString.Str(), *pJson);

        if (!result)
        {
            return false;
        }

        *ppJson = pJson;

        return true;
    }

    void Loaders::UnloadJSON(ResourceManager* pResourceManager, FileSystem* pFileSystem, JSON* pJson)
    {
        delete pJson;
    }

    Bool8 Loaders::LoadRawImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, RawImage** ppRawImage)
    {
        *ppRawImage = LoadImageSTB(path, true, true);
        return *ppRawImage != nullptr;
    }

    void Loaders::UnloadRawImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, RawImage* pRawImage)
    {
        FreeImageSTB(pRawImage);
    }

    Bool8 Loaders::LoadOBJModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, OBJModel** ppObjModel)
    {
        String modelData;
        Bool8 result; 

        result = ReadFileString(path, modelData);

        if (!result)
        {
            return false;
        }

        OBJModel* pObjModel = new OBJModel{};
        result = LoadOBJ(modelData, pObjModel);

        if (!result)
        {
            return false;
        }

        *ppObjModel = pObjModel;

        return result;
    }

    void Loaders::UnloadOBJModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, OBJModel* pObjModel)
    {
        delete pObjModel;
    }

    Bool8 Loaders::LoadGLTFModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, GLTFModel** ppGltfModel)
    {
        String jsonString;
        Bool8 result;

        result = ReadFileString(path, jsonString);

        if (!result)
        {
            return false;
        }

        JSON json;
        result = LoadJSON(jsonString.Str(), json);

        if (!result)
        {
            return false;
        }

        GLTFModel* pGltfModel = new GLTFModel();

        result = LoadGLTF(json, *pGltfModel);

        if (!result)
        {
            return false;
        }

        *ppGltfModel = pGltfModel;

        return true;
    }

    void Loaders::UnloadGLTFModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, GLTFModel* pGltfModel)
    {
        delete pGltfModel;
    }

    Bool8 Loaders::LoadSpirvObject(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, SpirvObject** ppSpirvObject)
    {
        SpirvObject* pSprivObject = new SpirvObject{};

        if (!LoadSpirv(path, pSprivObject))
        {
            delete pSprivObject;
            return false;
        }

        *ppSpirvObject = pSprivObject;

        return true;
    }

    void Loaders::UnloadSpirvObject(ResourceManager* pResourceManager, FileSystem* pFileSystem, SpirvObject* pSpirvObject)
    {
        delete pSpirvObject;
    }

    Bool8 Loaders::LoadShader(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Shader** ppShader)
    {
        String jsonString;
        Bool8 result;

        result = ReadFileString(path, jsonString);

        if (!result)
        {
            return false;
        }

        JSON json;
        result = LoadJSON(jsonString.Str(), json);

        if (!result)
        {
            return false;
        }

        ProgramType type;
        JSONValue jsonProgram = json.GetRoot()->Get("program");

        if (jsonProgram.IsNull() || !jsonProgram.IsString())
        {
            // No program type described, invalid file
            return false;
        }

        String programString = jsonProgram.AsString();

        if (programString == "traditional")
        {
            type = TRADITIONAL_PROGRAM;
        }
        else if (programString == "compute")
        {
            type = COMPUTE_PROGRAM;
        }
        else if (programString == "mesh")
        {
            type = MESH_PROGRAM;
        }
        else if (programString == "raytracing")
        {
            type = RAYTRACING_PROGRAM;
        }
        else
        {
            // Invalid program type
            return false;
        }

        JSONValue spirv = json.GetRoot()->Get("spirv");

        if (spirv.IsNull())
        {
            // No spirv array described, invalid file
            return false;
        }

        JSONObject spirvList = spirv.AsObject();

        Shader* pShader = new Shader();

        for (JSONObject::PairType& shader : spirvList)
        {
            SpirvObject* pSpirvObject = pResourceManager->CheckoutResource<SpirvObject>(shader.value.AsString());
            
            if (pSpirvObject)
            {
                pShader->spirvObjects.PushBack(pSpirvObject);
            }
            else
            {
                // Invalid spirv file

                for (SpirvObject* pSpirv : pShader->spirvObjects)
                {
                    pResourceManager->ReturnResource(pSpirv);
                }

                return false;
            }
        }

        *ppShader = pShader;

        return true;
    }

    void Loaders::UnloadShader(ResourceManager* pResourceManager, FileSystem* pFileSystem, Shader* pShader)
    {
        for (SpirvObject* pSpirv : pShader->spirvObjects)
        {
            pResourceManager->ReturnResource(pSpirv);
        }

        delete pShader;
    }

    Bool8 Loaders::LoadMaterial(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Material** ppMaterial)
    {
        String jsonString;
        Bool8 result;

        result = ReadFileString(path, jsonString);

        if (!result)
        {
            return false;
        }

        JSON json;
        result = LoadJSON(jsonString.Str(), json);

        if (!result)
        {
            return false;
        }

        Material* pMaterial = new Material();

        JSONValue name = json.GetRoot()->Get("name");
        JSONValue shader = json.GetRoot()->Get("shader");

        if (name.IsNull() || !name.IsString())
        {
            pMaterial->name = "New Material";
        }
        else
        {
            pMaterial->name = name.AsString();
        }

        if (shader.IsNull() || !shader.IsString())
        {
            // Invalid material
            return false;
        }
        else
        {
            Shader* pShader = pResourceManager->CheckoutResource<Shader>(shader.AsString());

            if (!pShader)
            {
                // Invalid shader in material
                return false;
            }

            pMaterial->pShader = pShader;
        }

        *ppMaterial = pMaterial;

        return true;
    }

    void Loaders::UnloadMaterial(ResourceManager* pResourceManager, FileSystem* pFileSystem, Material* pMaterial)
    {
        pResourceManager->ReturnResource(pMaterial->pShader);

        delete pMaterial;
    }

    void Loaders::RegisterCoreLoaders(ResourceManager* pResourceManager)
    {
        pResourceManager->RegisterResourceLoader<JSON>(LoadJSON, UnloadJSON);
        pResourceManager->RegisterResourceLoader<RawImage>(LoadRawImage, UnloadRawImage);
        pResourceManager->RegisterResourceLoader<OBJModel>(LoadOBJModel, UnloadOBJModel);
        pResourceManager->RegisterResourceLoader<GLTFModel>(LoadGLTFModel, UnloadGLTFModel);
        pResourceManager->RegisterResourceLoader<SpirvObject>(LoadSpirvObject, UnloadSpirvObject);
        pResourceManager->RegisterResourceLoader<Shader>(LoadShader, UnloadShader);
        pResourceManager->RegisterResourceLoader<Material>(LoadMaterial, UnloadMaterial);
    }
}

