#include "GraphicsLoaders.h"

#include "util/Buffer.h"

#include "../../resource/object/RawImage.h"
#include "../../Engine.h"

#include "../../resource/object/OBJ.h"

#include <math.h>

namespace Quartz
{
	template<>
	UInt32 Hash<OBJIndex>(const OBJIndex& index)
	{
		return Hash<UInt64>(index.Hash());
	}

	Bool8 Loaders::LoadImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Image** ppImage)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();
		RawImage* pRawImage = pResourceManager->CheckoutResource<RawImage>(path);

		if (!pRawImage)
		{
			Log::Error("Failed to load image path '%s;", path.Str());
			return false;
		}

		ImageFormat format;

		if (pRawImage->GetBitsPerChannel() == 8)
		{
			switch (pRawImage->GetChannels())
			{
				case 1: format = IMAGE_FORMAT_R;	break;
				case 2: format = IMAGE_FORMAT_RG;	break;
				case 3: format = IMAGE_FORMAT_RGB;	break;
				case 4: format = IMAGE_FORMAT_RGBA; break;
			}
		}
		else if (pRawImage->GetBitsPerChannel() == 16)
		{
			switch (pRawImage->GetChannels())
			{
				case 1: format = IMAGE_FORMAT_R16;		break;
				case 2: format = IMAGE_FORMAT_RG16;		break;
				case 3: format = IMAGE_FORMAT_RGB16;	break;
				case 4: format = IMAGE_FORMAT_RGBA16;	break;
			}
		}
		else if (pRawImage->GetBitsPerChannel() == 32)
		{
			switch (pRawImage->GetChannels())
			{
				case 1: format = IMAGE_FORMAT_R_FLOAT;		break;
				case 2: format = IMAGE_FORMAT_RG_FLOAT;		break;
				case 3: format = IMAGE_FORMAT_RGB_FLOAT;	break;
				case 4: format = IMAGE_FORMAT_RGBA_FLOAT;	break;
			}
		}
		else
		{
			Log::Error("Invalid RawImage paramaters: %d bits per channel!", pRawImage->GetBitsPerChannel());
			return false;
		}

		UInt32 mipLevels = static_cast<uint32_t>(std::floor(std::log2f(std::max(pRawImage->GetWidth(), pRawImage->GetHeight())))) + 1;
		UInt32 sizeBytes = pRawImage->GetSizeBytes();

		Image* pImage = pGraphics->CreateImage
		(
			IMAGE_TYPE_2D,
			pRawImage->GetWidth(), pRawImage->GetHeight(), 1, 1, mipLevels,
			format,
			IMAGE_USAGE_SAMPLED_TEXTURE_BIT | IMAGE_USAGE_TRANSFER_SRC_BIT | IMAGE_USAGE_TRANSFER_DST_BIT
		);

		Buffer* pStagingBuffer = pGraphics->CreateBuffer
		(
			sizeBytes,
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT
		);

		void* pStagingData = pStagingBuffer->MapBuffer(sizeBytes, 0);
		memcpy(pStagingData, pRawImage->GetData(), sizeBytes);
		pStagingBuffer->UnmapBuffer();

		pGraphics->CopyBufferToImage(pStagingBuffer, pImage);

		pGraphics->DestroyBuffer(pStagingBuffer);

		pGraphics->GenerateMips(pImage);

		pResourceManager->ReturnResource(pRawImage);

		*ppImage = pImage;

		return true;
	}

	void Loaders::UnloadImage(ResourceManager* pResourceManager, FileSystem* pFileSystem, Image* pImage)
	{
		Engine::GetInstance()->GetGraphics()->DestroyImage(pImage);
	}

	Bool8 Loaders::LoadModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, const String& path, Model** ppModel)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		String ext = pResourceManager->GetExtension(path);

		if (ext == "obj")
		{
			Model* pModel = new Model{};
			
			OBJModel* pObjModel = pResourceManager->CheckoutResource<OBJModel>(path);

			if (!pObjModel)
			{
				Log::Error("Failed to model path '%s;", path.Str());
				return false;
			}

			OBJModel& objModel = *pObjModel;

			UInt32 indexByteSize = objModel.indices.Size() * sizeof(UInt32);
			UInt32 vertexByteSize = objModel.indices.Size() * 11 * sizeof(Float32);

			Buffer* pVertexStagingBuffer = pGraphics->CreateBuffer
			(
				vertexByteSize,
				BUFFER_USAGE_TRANSFER_SRC_BIT,
				BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT
			);

			Buffer* pIndexStagingBuffer = pGraphics->CreateBuffer
			(
				indexByteSize,
				BUFFER_USAGE_TRANSFER_SRC_BIT,
				BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT
			);

			pModel->pVertexBuffer = pGraphics->CreateBuffer
			(
				vertexByteSize,
				BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
				BUFFER_ACCESS_DEVICE_LOCAL_BIT
			);

			pModel->pIndexBuffer = pGraphics->CreateBuffer
			(
				indexByteSize,
				BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
				BUFFER_ACCESS_DEVICE_LOCAL_BIT
			);

			Float32* pVertexData	= (Float32*)pVertexStagingBuffer->MapBuffer(pVertexStagingBuffer->GetSize(), 0);
			UInt32*  pIndexData		= (UInt32*)pIndexStagingBuffer->MapBuffer(pIndexStagingBuffer->GetSize(), 0);

			UInt64 vertexOffset = 0;
			UInt64 indexOffset	= 0;
			UInt32 currentIndex = 0;

			Map<OBJIndex, UInt32> indexMap{};
			indexMap.Reserve(objModel.indices.Size() * 1.5f);

			for (UInt32 i = 0; i < (objModel.indices.Size() - 2); i += 3)
			{
				const OBJIndex& index0 = objModel.indices[i];
				const OBJIndex& index1 = objModel.indices[i + 1];
				const OBJIndex& index2 = objModel.indices[i + 2];

				//Vector3 binormal = Vector3(0, 0, 0);
				Vector3 tangent = Vector3(0, 0, 0);

				if (objModel.texCoords.Size() > 0 &&
					index0.texCoordIdx != 0 &&
					index1.texCoordIdx != 0 &&
					index2.texCoordIdx != 0)
				{
					// Generate tangents *per-face*

					Vector3 deltaPos1 = objModel.positions[index1.positionIdx - 1] - objModel.positions[index0.positionIdx - 1];
					Vector3 deltaPos2 = objModel.positions[index2.positionIdx - 1] - objModel.positions[index1.positionIdx - 1];
					Vector2 deltaTex1 = objModel.texCoords[index1.texCoordIdx - 1] - objModel.texCoords[index0.texCoordIdx - 1];
					Vector2 deltaTex2 = objModel.texCoords[index2.texCoordIdx - 1] - objModel.texCoords[index1.texCoordIdx - 1];

					float f = 1.0f / (deltaTex1.x * deltaTex2.y - deltaTex2.x * deltaTex1.y);

					tangent.x = f * (deltaTex2.y * deltaPos1.x - deltaTex1.y * deltaPos2.x);
					tangent.y = f * (deltaTex2.y * deltaPos1.y - deltaTex1.y * deltaPos2.y);
					tangent.z = f * (deltaTex2.y * deltaPos1.z - deltaTex1.y * deltaPos2.z);
				}

				for (UInt32 j = 0; j < 3; j++)
				{
					const OBJIndex& index = objModel.indices[i + j];
					UInt32* pIdx = indexMap.Get(index);

					if (pIdx != nullptr)
					{
						pIndexData[indexOffset++] = *pIdx;
					}
					else
					{
						Vector3& position = objModel.positions[index.positionIdx - 1];
						pVertexData[vertexOffset++] = position.x;
						pVertexData[vertexOffset++] = position.y;
						pVertexData[vertexOffset++] = position.z;

						if (objModel.normals.Size() > 0 && index.normalIdx != 0)
						{
							Vector3& normal = objModel.normals[index.normalIdx - 1];
							pVertexData[vertexOffset++] = normal.x;
							pVertexData[vertexOffset++] = normal.y;
							pVertexData[vertexOffset++] = normal.z;
						}
						else
						{
							// Generate normals
							Vector3 normal = Vector3(0, 0, 0);
							pVertexData[vertexOffset++] = normal.x;
							pVertexData[vertexOffset++] = normal.y;
							pVertexData[vertexOffset++] = normal.z;
						}

						pVertexData[vertexOffset++] = tangent.x;
						pVertexData[vertexOffset++] = tangent.y;
						pVertexData[vertexOffset++] = tangent.z;

						if (objModel.texCoords.Size() > 0 && index.texCoordIdx != 0)
						{
							Vector2& texCoord = objModel.texCoords[index.texCoordIdx - 1];
							pVertexData[vertexOffset++] = texCoord.x;
							pVertexData[vertexOffset++] = texCoord.y;
						}
						else
						{
							// Generate texCoords
							Vector2 texCoord = Vector2(0, 0);
							pVertexData[vertexOffset++] = texCoord.x;
							pVertexData[vertexOffset++] = texCoord.y;
						}

						indexMap.Put(index, currentIndex);
						pIndexData[indexOffset++] = currentIndex++;
					}
				}
			}

			pVertexStagingBuffer->UnmapBuffer();
			pIndexStagingBuffer->UnmapBuffer();

			pGraphics->CopyBuffer(pVertexStagingBuffer, pModel->pVertexBuffer);
			pGraphics->CopyBuffer(pIndexStagingBuffer, pModel->pIndexBuffer);

			pGraphics->DestroyBuffer(pVertexStagingBuffer);
			pGraphics->DestroyBuffer(pIndexStagingBuffer);

			pResourceManager->ReturnResource(pObjModel);

			*ppModel = pModel;

			return true;
		}

		return false;
	}

	void Loaders::UnloadModel(ResourceManager* pResourceManager, FileSystem* pFileSystem, Model* pModel)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		pGraphics->DestroyBuffer(pModel->pVertexBuffer);
		pGraphics->DestroyBuffer(pModel->pIndexBuffer);

		delete pModel;
	}

	void Loaders::RegisterGraphicsLoaders(ResourceManager* pResourceManager)
	{
		pResourceManager->RegisterResourceLoader<Image>(LoadImage, UnloadImage);
		pResourceManager->RegisterResourceLoader<Model>(LoadModel, UnloadModel);
	}
}

