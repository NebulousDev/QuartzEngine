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

	void SimpleRenderer::Setup(Viewport* pViewport)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		Renderpass* pRenderpass = pGraphics->CreateRenderpass
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

		Framebuffer* pFramebuffer = pGraphics->CreateFramebuffer(pRenderpass, pViewport, 
			pViewport->GetWidth(), pViewport->GetHeight());

		Shader* pVertexShader	= pGraphics->CreateShader("Vertex", ReadFile("assets/shaders/flat_vert.spv"));
		Shader* pFragmentShader = pGraphics->CreateShader("Fragment", ReadFile("assets/shaders/flat_frag.spv"));

		GraphicsPipelineInfo pipelineInfo = {};
		{
			pipelineInfo.shaders =
			{
				pVertexShader,
				pFragmentShader
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
			pipelineInfo.cullMode			= CULL_MODE_BACK;
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

			pipelineInfo.pRenderpass = pRenderpass;
		}

		GraphicsPipeline* pGraphicsPipeline = pGraphics->CreateGraphicsPipeline(pipelineInfo, 0);

		/*=====================================
			MESH BUFFERS
		=====================================*/

		Array<Byte> modelFileData	= ReadFile(MODEL_PATH);
		String modelDataString		= String((char*)modelFileData.Data(), modelFileData.Size());
		Model mModel				= LoadOBJ(modelDataString);

		Buffer* pVertexStagingBuffer = pGraphics->CreateBuffer(
			mModel.vertexData.buffer.Size(), 
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		Buffer* pIndexStagingBuffer = pGraphics->CreateBuffer(
			mModel.indexData.buffer.Size(),
			BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		Buffer* pVertexBuffer = pGraphics->CreateBuffer(
			mModel.vertexData.buffer.Size(),
			BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT);

		Buffer* pIndexBuffer = pGraphics->CreateBuffer(
			mModel.indexData.buffer.Size(),
			BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT,
			BUFFER_ACCESS_DEVICE_LOCAL_BIT);

		void* pVertexData = pVertexStagingBuffer->MapBuffer(pVertexStagingBuffer->GetSize(), 0);
		memcpy(pVertexData, mModel.vertexData.buffer.Data(), mModel.vertexData.buffer.Size());
		pVertexStagingBuffer->UnmapBuffer();

		void* pIndexData = pIndexStagingBuffer->MapBuffer(pIndexStagingBuffer->GetSize(), 0);
		memcpy(pIndexData, mModel.indexData.buffer.Data(), mModel.indexData.buffer.Size());
		pIndexStagingBuffer->UnmapBuffer();

		pGraphics->CopyBuffer(pVertexStagingBuffer, pVertexBuffer);
		pGraphics->CopyBuffer(pIndexStagingBuffer, pIndexBuffer);

		pGraphics->DestroyBuffer(pVertexStagingBuffer);
		pGraphics->DestroyBuffer(pIndexStagingBuffer);

		/*=====================================
			UNIFORM BUFFERS
		=====================================*/

		struct PerFrameUBO
		{
			Matrix4 model;
			Matrix4 view;
			Matrix4 proj;
		}
		perFrameUbo{};

		struct PerObjectUBO
		{
			Matrix4 model;
		};

		constexpr UInt32 numPerObjectUBOs = 10000;

		//UniformArray* pPerObjectUBO = pGraphics->CreateUniformArray(UInt32 size, UInt32 count, BufferUsage, BufferAccess);
		//Uniform* pPerFrameUBO = pGraphics->CreateUniform(UInt32 size, BufferUsage, BufferAccess);

		Buffer* pPerFrameUBO = pGraphics->CreateBuffer(
			sizeof(perFrameUbo),
			BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		Buffer* pPerObjectUBO = pGraphics->CreateBuffer(
			sizeof(perFrameUbo) * numPerObjectUBOs,
			BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_TRANSFER_SRC_BIT,
			BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT);

		/*=====================================
			COMMAND BUFFER
		=====================================*/

		mpCommandBuffer = pGraphics->CreateCommandBuffer();

		{
			mpCommandBuffer->BeginRecording();
			mpCommandBuffer->BeginRenderpass(pRenderpass, pFramebuffer);
			mpCommandBuffer->SetPipeline(pGraphicsPipeline);

			for (UInt32 i = 0; i < 1; i++)
			{
				mpCommandBuffer->SetVertexBuffers({ pVertexBuffer });
				mpCommandBuffer->SetIndexBuffer(pIndexBuffer);
				mpCommandBuffer->DrawIndexed(pIndexBuffer->GetSize() / sizeof(UInt32), 0);
			}

			mpCommandBuffer->EndRenderpass();
			mpCommandBuffer->EndRecording();
		}
	}

	void SimpleRenderer::Render(Viewport* pViewport, Scene* pScene)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		pGraphics->Submit(pViewport, mpCommandBuffer);

		int temp;
	}

	void SimpleRenderer::Rebuild(Viewport* pViewport)
	{

	}

	void SimpleRenderer::Destroy()
	{

	}
}

