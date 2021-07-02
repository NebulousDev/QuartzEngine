#include "SimpleRenderer.h"

#include "../../Engine.h"
#include "../../loaders/OBJLoader.h"

#include <iostream>
#include <fstream>

#include "../../log/Log.h"

/*=====================================
	SCENE RESOURCES
=====================================*/

#define MODEL_PATH		"assets/models/testScene.obj"
#define MODEL_PATH2		"assets/models/dragon.obj"
#define DIFFUSE_PATH	"assets/textures/stone.png"
#define NORMAL_PATH		"assets/textures/stone_normal.png"
#define SPECULAR_PATH	"assets/textures/stone_specular.png"

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

	SimpleRenderer::SimpleRenderer()
	{
		// Nothing
	}

	void SimpleRenderer::Setup(Context* pViewport)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		mpRenderpass = pGraphics->CreateRenderpass
		(
			"Color-Pass",

			{
				{ "Swapchain",		ATTACHMENT_SWAPCHAIN,		IMAGE_FORMAT_BGRA },
				{ "Depth-Stencil",	ATTACHMENT_DEPTH_STENCIL,	IMAGE_FORMAT_DEPTH_24_STENCIL_8 }
			},

			{
				{ "Color-Subpass", { 0, 1 } }
			}
		);

		mpFramebuffer = pGraphics->CreateFramebuffer(mpRenderpass, pViewport, 
			pViewport->GetWidth(), pViewport->GetHeight());

		mpVertexShader	= pGraphics->CreateShader("Vertex", ReadFile("assets/shaders/flat_vert.spv"));
		mpFragmentShader = pGraphics->CreateShader("Fragment", ReadFile("assets/shaders/flat_frag.spv"));

		GraphicsPipelineInfo pipelineInfo = {};
		{
			pipelineInfo.shaders =
			{
				mpVertexShader,
				mpFragmentShader
			};

			pipelineInfo.viewport.x			= 0;
			pipelineInfo.viewport.y			= 0;
			pipelineInfo.viewport.width		= pViewport->GetWidth();
			pipelineInfo.viewport.height	= pViewport->GetHeight();
			pipelineInfo.viewport.minDepth	= 0.0f;
			pipelineInfo.viewport.maxDepth	= 1.0f;

			pipelineInfo.scissor.x			= 0;
			pipelineInfo.scissor.y			= 0;
			pipelineInfo.scissor.width		= pViewport->GetWidth();
			pipelineInfo.scissor.height		= pViewport->GetHeight();

			pipelineInfo.topology			= PRIMITIVE_TOPOLOGY_TRIANGLES;
			pipelineInfo.polygonMode		= POLYGON_MODE_FILL;
			pipelineInfo.cullMode			= CULL_MODE_NONE;
			pipelineInfo.faceWind			= FACE_WIND_COUNTER_CLOCKWISE;
			pipelineInfo.lineWidth			= 1.0f;

			pipelineInfo.multisample		= MULTISAMPLE_DISABLED;

			pipelineInfo.depthTesting		= true;
			pipelineInfo.depthOperation		= COMPARE_OP_LESS_OR_EQUAL;

			BufferAttachent vertexBufferAttachment;
			vertexBufferAttachment.binding	= 0;
			vertexBufferAttachment.stride	= 14 * sizeof(Float32);

			pipelineInfo.bufferAttachments.PushBack(vertexBufferAttachment);

			VertexAttribute positionAttrib;
			positionAttrib.binding	= 0;
			positionAttrib.location	= 0;
			positionAttrib.type		= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute normalAttrib;
			normalAttrib.binding	= 0;
			normalAttrib.location	= 1;
			normalAttrib.type		= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute texCoordAttrib;
			texCoordAttrib.binding	= 0;
			texCoordAttrib.location = 2;
			texCoordAttrib.type		= ATTRIBUTE_TYPE_FLOAT2;

			pipelineInfo.vertexAttributes.PushBack(positionAttrib);
			pipelineInfo.vertexAttributes.PushBack(normalAttrib);
			pipelineInfo.vertexAttributes.PushBack(texCoordAttrib);

			BlendAttachment colorOutputBlendAttachment;
			colorOutputBlendAttachment.blend = ColorBlend();
			pipelineInfo.blendAttachments.PushBack(colorOutputBlendAttachment);

			pipelineInfo.pRenderpass = mpRenderpass;
		}

		mpGraphicsPipeline = pGraphics->CreateGraphicsPipeline(pipelineInfo, 0);

		/*=====================================
			MESH BUFFERS
		=====================================*/

		Array<Byte> modelFileData	= ReadFile(MODEL_PATH);
		String modelDataString		= String((char*)modelFileData.Data(), modelFileData.Size());
		Model mModel				= LoadOBJ(modelDataString);

		Array<Byte> modelFileData2	= ReadFile(MODEL_PATH2);
		String modelDataString2		= String((char*)modelFileData2.Data(), modelFileData2.Size());
		Model mModel2				= LoadOBJ(modelDataString2);

		Buffer* pVertexStagingBuffer = pGraphics->CreateBuffer(
			mModel.vertexData.buffer.Size(), 
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		Buffer* pIndexStagingBuffer = pGraphics->CreateBuffer(
			mModel.indexData.buffer.Size(),
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		Buffer* pVertexStagingBuffer2 = pGraphics->CreateBuffer(
			mModel2.vertexData.buffer.Size(),
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		Buffer* pIndexStagingBuffer2 = pGraphics->CreateBuffer(
			mModel2.indexData.buffer.Size(),
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		mpVertexBuffer = pGraphics->CreateBuffer(
			mModel.vertexData.buffer.Size(),
			BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT);

		mpIndexBuffer = pGraphics->CreateBuffer(
			mModel.indexData.buffer.Size(),
			BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT);

		mpVertexBuffer2 = pGraphics->CreateBuffer(
			mModel2.vertexData.buffer.Size(),
			BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT);

		mpIndexBuffer2 = pGraphics->CreateBuffer(
			mModel2.indexData.buffer.Size(),
			BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT);

		void* pVertexData = pVertexStagingBuffer->MapBuffer(pVertexStagingBuffer->GetSize(), 0);
		memcpy(pVertexData, mModel.vertexData.buffer.Data(), mModel.vertexData.buffer.Size());
		pVertexStagingBuffer->UnmapBuffer();

		void* pIndexData = pIndexStagingBuffer->MapBuffer(pIndexStagingBuffer->GetSize(), 0);
		memcpy(pIndexData, mModel.indexData.buffer.Data(), mModel.indexData.buffer.Size());
		pIndexStagingBuffer->UnmapBuffer();

		void* pVertexData2 = pVertexStagingBuffer2->MapBuffer(pVertexStagingBuffer2->GetSize(), 0);
		memcpy(pVertexData2, mModel2.vertexData.buffer.Data(), mModel2.vertexData.buffer.Size());
		pVertexStagingBuffer2->UnmapBuffer();

		void* pIndexData2 = pIndexStagingBuffer2->MapBuffer(pIndexStagingBuffer2->GetSize(), 0);
		memcpy(pIndexData2, mModel2.indexData.buffer.Data(), mModel2.indexData.buffer.Size());
		pIndexStagingBuffer2->UnmapBuffer();

		pGraphics->CopyBuffer(pVertexStagingBuffer, mpVertexBuffer);
		pGraphics->CopyBuffer(pIndexStagingBuffer, mpIndexBuffer);

		pGraphics->CopyBuffer(pVertexStagingBuffer2, mpVertexBuffer2);
		pGraphics->CopyBuffer(pIndexStagingBuffer2, mpIndexBuffer2);

		pGraphics->DestroyBuffer(pVertexStagingBuffer);
		pGraphics->DestroyBuffer(pIndexStagingBuffer);

		pGraphics->DestroyBuffer(pVertexStagingBuffer2);
		pGraphics->DestroyBuffer(pIndexStagingBuffer2);

		/*=====================================
			UNIFORM BUFFERS
		=====================================*/

		mpPerFrame	= pGraphics->CreateUniform(UNIFORM_DYNAMIC, sizeof(PerFrameUBO), 1, 0);
		mpPerObject = pGraphics->CreateUniform(UNIFORM_DYNAMIC, sizeof(PerObjectUBO), 2, 0);

		/*=====================================
			COMMAND BUFFER
		=====================================*/

		mpCommandBuffer = pGraphics->CreateCommandBuffer(COMMAND_BUFFER_DYNAMIC);
	}

	void SimpleRenderer::Render(Context* pViewport, Scene* pScene)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		// Fill uniform buffers
		{
			perFrameUbo.view	= Matrix4().SetTranslation({ 0.0f, 0.0f, 0.0f });
			perFrameUbo.proj	= Matrix4().SetPerspective(90.0f, 1920.0f / 1080.0f, 0.001f, 1000.0f);
			mpPerFrame->SetElement(pViewport, 0, &perFrameUbo);

			for (UInt32 i = 0; i < 2; i++)
			{
				perObjectUbo.model = Matrix4().SetIdentity();
				mpPerObject->SetElement(pViewport, i, &perObjectUbo);
			}
		}

		// Fill command buffer
		{
			mpCommandBuffer->BeginRecording();
			mpCommandBuffer->BeginRenderpass(mpRenderpass, mpFramebuffer);
			mpCommandBuffer->SetPipeline(mpGraphicsPipeline);
			mpCommandBuffer->BindUniform(0, 0, mpPerFrame, 0);

				mpCommandBuffer->SetVertexBuffers({ mpVertexBuffer });
				mpCommandBuffer->SetIndexBuffer(mpIndexBuffer);
				mpCommandBuffer->BindUniform(1, 0, mpPerObject, 0);
				mpCommandBuffer->DrawIndexed(mpIndexBuffer->GetSize() / sizeof(UInt32), 0);

				mpCommandBuffer->SetVertexBuffers({ mpVertexBuffer2 });
				mpCommandBuffer->SetIndexBuffer(mpIndexBuffer2);
				mpCommandBuffer->BindUniform(1, 0, mpPerObject, 1);
				mpCommandBuffer->DrawIndexed(mpIndexBuffer2->GetSize() / sizeof(UInt32), 0);

			mpCommandBuffer->EndRenderpass();
			mpCommandBuffer->EndRecording();
		}

		// Submit
		pGraphics->Submit(pViewport, mpCommandBuffer);

		int temp;
	}

	void SimpleRenderer::Rebuild(Context* pViewport)
	{

	}

	void SimpleRenderer::Destroy()
	{

	}
}

