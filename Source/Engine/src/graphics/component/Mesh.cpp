#include "Mesh.h"

#include "../../loaders/OBJLoader.h"
#include "../../log/Log.h"

#include <iostream>
#include <fstream>

#include "../../Engine.h"

namespace Quartz
{
	static Array<Byte> ReadFile(const String& filename)
	{
		std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			Log::Error("Cannot open file %s", filename.Str());
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		Array<Byte> buffer(fileSize);

		file.seekg(0);
		file.read((char*)buffer.Data(), fileSize);

		file.close();

		return buffer;
	}

	// TODO: Note this is all temporary until I have a proper buffer manager
	MeshComponent::MeshComponent(const String& filepath)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		Array<Byte> modelFileData = ReadFile(filepath);
		String modelDataString = String((char*)modelFileData.Data(), modelFileData.Size());
		Model model = LoadOBJ(modelDataString);

		Buffer* pVertexStagingBuffer = pGraphics->CreateBuffer
		(
			model.vertexData.buffer.Size(),
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT
		);

		Buffer* pIndexStagingBuffer = pGraphics->CreateBuffer
		(
			model.indexData.buffer.Size(),
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT
		);

		pVertexBuffer = pGraphics->CreateBuffer
		(
			model.vertexData.buffer.Size(),
			BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT
		);

		pIndexBuffer = pGraphics->CreateBuffer
		(
			model.indexData.buffer.Size(),
			BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT
		);

		void* pVertexData = pVertexStagingBuffer->MapBuffer(pVertexStagingBuffer->GetSize(), 0);
		memcpy(pVertexData, model.vertexData.buffer.Data(), model.vertexData.buffer.Size());
		pVertexStagingBuffer->UnmapBuffer();

		void* pIndexData = pIndexStagingBuffer->MapBuffer(pIndexStagingBuffer->GetSize(), 0);
		memcpy(pIndexData, model.indexData.buffer.Data(), model.indexData.buffer.Size());
		pIndexStagingBuffer->UnmapBuffer();

		pGraphics->CopyBuffer(pVertexStagingBuffer, pVertexBuffer);
		pGraphics->CopyBuffer(pIndexStagingBuffer, pIndexBuffer);

		pGraphics->DestroyBuffer(pVertexStagingBuffer);
		pGraphics->DestroyBuffer(pIndexStagingBuffer);
	}
}

