#include "SimpleRenderer.h"

#include "../../Engine.h"
#include "../../loaders/OBJLoader.h"

#include <iostream>
#include <fstream>

#include "../../log/Log.h"

#include "../../entity/basic/Transform.h"
#include "../component/Mesh.h"
#include "../component/Camera.h"
#include "../component/Material.h"

/*=====================================
	SCENE RESOURCES
=====================================*/

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
			positionAttrib.binding		= 0;
			positionAttrib.location		= 0;
			positionAttrib.type			= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute normalAttrib;
			normalAttrib.binding		= 0;
			normalAttrib.location		= 1;
			normalAttrib.type			= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute binormalAttrib;
			binormalAttrib.binding		= 0;
			binormalAttrib.location		= 2;
			binormalAttrib.type			= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute bitangentAttrib;
			bitangentAttrib.binding		= 0;
			bitangentAttrib.location	= 3;
			bitangentAttrib.type		= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute texCoordAttrib;
			texCoordAttrib.binding		= 0;
			texCoordAttrib.location		= 4;
			texCoordAttrib.type			= ATTRIBUTE_TYPE_FLOAT2;

			pipelineInfo.vertexAttributes.PushBack(positionAttrib);
			pipelineInfo.vertexAttributes.PushBack(normalAttrib);
			pipelineInfo.vertexAttributes.PushBack(binormalAttrib);
			pipelineInfo.vertexAttributes.PushBack(bitangentAttrib);
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

		// Moved to MeshComponents

		/*=====================================
			UNIFORM BUFFERS
		=====================================*/

		mpPerFrame	= pGraphics->CreateUniform(UNIFORM_DYNAMIC, sizeof(PerFrameUBO), 1, 0);
		mpPerObject = pGraphics->CreateUniform(UNIFORM_DYNAMIC, sizeof(PerObjectUBO), 64, 0);

		mpDiffuse	= pGraphics->CreateUniformTextureSampler();
		mpNormal	= pGraphics->CreateUniformTextureSampler();
		mpRoughness	= pGraphics->CreateUniformTextureSampler();
		mpMetallic	= pGraphics->CreateUniformTextureSampler();
		mpAmbient	= pGraphics->CreateUniformTextureSampler();

		/*=====================================
			COMMAND BUFFER
		=====================================*/

		mpCommandBuffer = pGraphics->CreateCommandBuffer(COMMAND_BUFFER_DYNAMIC);
	}

	void SimpleRenderer::Render(Context* pViewport, Scene* pScene)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();
		EntityWorld& world = pScene->GetWorld();

		EntityView renderables = world.CreateView<TransformComponent, MeshComponent, MaterialComponent>();

		TransformComponent& cameraTransform = pScene->GetWorld().GetComponent<TransformComponent>(pScene->GetCamera());
		CameraComponent&	cameraCamera	= pScene->GetWorld().GetComponent<CameraComponent>(pScene->GetCamera());

		// Fill uniform buffers
		{
			perFrameUbo.view = cameraTransform.GetMatrix();
			perFrameUbo.proj = cameraCamera.perspective;
			perFrameUbo.cameraPos = cameraTransform.position;

			mpPerFrame->SetElement(pViewport, 0, &perFrameUbo);

			UInt32 i = 0;
			for (Entity entity : renderables)
			{
				TransformComponent& transform = pScene->GetWorld().GetComponent<TransformComponent>(entity);
				perObjectUbo.model = transform.GetMatrix();
				mpPerObject->SetElement(pViewport, i++, &perObjectUbo);
			}
		}

		// Fill command buffer
		{
			mpCommandBuffer->BeginRecording();
			mpCommandBuffer->BeginRenderpass(mpRenderpass, mpFramebuffer);
			mpCommandBuffer->SetPipeline(mpGraphicsPipeline);
			mpCommandBuffer->BindUniform(0, 0, mpPerFrame, 0);

			UInt32 i = 0;
			for (Entity entity : renderables)
			{
				MeshComponent& mesh = pScene->GetWorld().GetComponent<MeshComponent>(entity);
				MaterialComponent& material = pScene->GetWorld().GetComponent<MaterialComponent>(entity);

				mpDiffuse->Set(material.pDiffuse);
				mpNormal->Set(material.pNormal);
				mpRoughness->Set(material.pRoughness);
				mpMetallic->Set(material.pMetallic);
				mpAmbient->Set(material.pAmbient);

				mpCommandBuffer->BindUniformTexture(2, 1, mpDiffuse);
				mpCommandBuffer->BindUniformTexture(2, 2, mpNormal);
				mpCommandBuffer->BindUniformTexture(2, 3, mpRoughness);
				mpCommandBuffer->BindUniformTexture(2, 4, mpMetallic);
				mpCommandBuffer->BindUniformTexture(2, 5, mpAmbient);

				mpCommandBuffer->SetVertexBuffers({ mesh.pVertexBuffer });
				mpCommandBuffer->SetIndexBuffer(mesh.pIndexBuffer);
				mpCommandBuffer->BindUniform(1, 0, mpPerObject, i++);
				mpCommandBuffer->DrawIndexed(mesh.pIndexBuffer->GetSize() / sizeof(UInt32), 0);
			}

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

