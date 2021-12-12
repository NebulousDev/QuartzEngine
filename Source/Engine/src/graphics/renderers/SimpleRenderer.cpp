#include "SimpleRenderer.h"

#include "../../Engine.h"
#include "../../resource/loaders/OBJLoader.h"

#include <iostream>
#include <fstream>

#include "../../log/Log.h"

#include "../component/Mesh.h"
#include "../component/Camera.h"
#include "../component/Material.h"
#include "../component/Light.h"

/*=====================================
	SCENE RESOURCES
=====================================*/

#define UNIFORM_OBJECT_SET_SIZE 64

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
			pipelineInfo.cullMode			= CULL_MODE_BACK;
			pipelineInfo.faceWind			= FACE_WIND_COUNTER_CLOCKWISE;
			pipelineInfo.lineWidth			= 1.0f;

			pipelineInfo.multisample		= MULTISAMPLE_DISABLED;

			pipelineInfo.depthTesting		= true;
			pipelineInfo.depthOperation		= COMPARE_OP_LESS_OR_EQUAL;

			BufferAttachent vertexBufferAttachment;
			vertexBufferAttachment.binding	= 0;
			vertexBufferAttachment.stride	= 11 * sizeof(Float32);

			pipelineInfo.bufferAttachments.PushBack(vertexBufferAttachment);

			VertexAttribute positionAttrib;
			positionAttrib.binding		= 0;
			positionAttrib.location		= 0;
			positionAttrib.type			= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute normalAttrib;
			normalAttrib.binding		= 0;
			normalAttrib.location		= 1;
			normalAttrib.type			= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute tangentAttrib;
			tangentAttrib.binding		= 0;
			tangentAttrib.location		= 2;
			tangentAttrib.type			= ATTRIBUTE_TYPE_FLOAT3;

			VertexAttribute texCoordAttrib;
			texCoordAttrib.binding		= 0;
			texCoordAttrib.location		= 3;
			texCoordAttrib.type			= ATTRIBUTE_TYPE_FLOAT2;

			pipelineInfo.vertexAttributes.PushBack(positionAttrib);
			pipelineInfo.vertexAttributes.PushBack(normalAttrib);
			pipelineInfo.vertexAttributes.PushBack(tangentAttrib);
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

		mpPerFrame	= pGraphics->CreateUniform(UNIFORM_DYNAMIC, sizeof(PerFrameUBOData), 1, 0);
		mpPerObject = pGraphics->CreateUniform(UNIFORM_DYNAMIC, sizeof(PerFrameObjectData), UNIFORM_OBJECT_SET_SIZE, 0);

		mPerSetObjects.Resize(UNIFORM_OBJECT_SET_SIZE);

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

	void RebuildEntityList(EntityWorld& world, Array<Entity>& outEntities)
	{
		EntityView renderables = world.CreateView<TransformComponent, MeshComponent, MaterialComponent>();

		outEntities.Clear();

		for (Entity entity : renderables)
		{
			outEntities.PushBack(entity);
		}
	}

	void FillPerFrameUniforms(EntityWorld& world, EntityGraph& graph, Entity camera, PerFrameUBOData& outData)
	{
		TransformComponent& cameraTransform = world.GetComponent<TransformComponent>(camera);
		CameraComponent&	cameraCamera	= world.GetComponent<CameraComponent>(camera);
		EntityView			lights			= world.CreateView<TransformComponent, LightComponent>();
		EntityView			renderables		= world.CreateView<TransformComponent, MeshComponent, MaterialComponent>();

		// Calculate Camera View
		Matrix4 cameraGlobal	= graph.GetNode(camera)->globalTransform;
		Matrix4 cameraParentInv	= graph.GetNode(camera)->pParent->globalTransform.Inverse();
		Matrix4 cameraView		= cameraParentInv * Matrix4().SetTranslation(-cameraTransform.position) * Matrix4().SetRotation(cameraTransform.rotation);
		Vector3 cameraPos		= cameraGlobal.GetTranslation();

		// Fill Per-frame UBO values
		outData.view		= cameraView;
		outData.proj		= cameraCamera.perspective;
		outData.cameraPos	= cameraPos;
		outData.lightCount	= 0;

		// Fill pre-computed light uniforms
		for (Entity entity : lights)
		{
			LightComponent& light = world.GetComponent<LightComponent>(entity);

			Vector3 transformedLightPosition = graph.GetNode(entity)->globalTransform.GetTranslation();

			outData.lights[outData.lightCount].position = transformedLightPosition;
			outData.lights[outData.lightCount].radiance = light.radiance;
			outData.lightCount++;

			// Only 16 lights supported for now
			if (outData.lightCount > 15) break;
		}
	}

	void FillPerObjectSetUniforms(EntityGraph& graph, Array<Entity> objectEntities, Array<PerFrameObjectData>& outData, UInt32 setID)
	{
		// Calc bounds for current object set
		UInt32 startIndex	= (setID + 0) * UNIFORM_OBJECT_SET_SIZE;
		UInt32 endIndex		= (setID + 1) * UNIFORM_OBJECT_SET_SIZE;

		if (endIndex > objectEntities.Size())
		{
			endIndex = objectEntities.Size();
		}

		// Fill pre-computed entity uniforms
		for (UInt32 i = startIndex; i < endIndex; i++)
		{
			Entity entity = objectEntities[i];
			outData[i].model = graph.GetNode(entity)->globalTransform;
		}
	}

	void SimpleRenderer::Render(Context* pViewport, Scene* pScene)
	{
		Graphics* pGraphics	= Engine::GetInstance()->GetGraphics();
		EntityWorld& world	= pScene->GetWorld();
		EntityGraph& graph	= world.GetGraph();
		Entity camera		= pScene->GetCamera();

		static Array<Entity> renderableObjects(UNIFORM_OBJECT_SET_SIZE);

		RebuildEntityList(world, renderableObjects);

		FillPerFrameUniforms(world, graph, camera, mPerFrameData);
		FillPerObjectSetUniforms(graph, renderableObjects, mPerSetObjects, 0);

		UInt32 objectCount = renderableObjects.Size(); // TEMPPPP!!! Only works sub max set

		mpPerFrame->SetElement(pViewport, 0, &mPerFrameData);
		mpPerObject->SetElementArray(pViewport, 0, objectCount, mPerSetObjects.Data());

		// Fill command buffer
		{
			mpCommandBuffer->BeginRecording();
			mpCommandBuffer->BeginRenderpass(mpRenderpass, mpFramebuffer);
			mpCommandBuffer->SetPipeline(mpGraphicsPipeline);
			mpCommandBuffer->BindUniform(0, 0, mpPerFrame, 0);

			UInt32 i = 0;
			for (Entity entity : renderableObjects)
			{
				//temp
				if (i > UNIFORM_OBJECT_SET_SIZE) break;

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

				mpCommandBuffer->SetVertexBuffers({ mesh.pModel->pVertexBuffer });
				mpCommandBuffer->SetIndexBuffer(mesh.pModel->pIndexBuffer);
				mpCommandBuffer->BindUniform(1, 0, mpPerObject, i++);
				mpCommandBuffer->DrawIndexed(mesh.pModel->pIndexBuffer->GetSize() / sizeof(UInt32), 0);
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

