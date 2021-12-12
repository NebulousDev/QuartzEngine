#include "GLTFLoader.h"

#include "../../log/Log.h"

namespace Quartz
{
	Bool8 ParseGLTFAsset(JSONValue& gltf, GLTFAsset& asset)
	{
		JSONValue gltfAsset = gltf["asset"];

		JSONValue gltfVersion		= gltfAsset["version"];
		JSONValue gltfMinVersion	= gltfAsset["minVersion"];
		JSONValue gltfGenerator		= gltfAsset["generator"];

		if (!gltfVersion.IsNull())
		{
			asset.version = gltfVersion.AsString();
		}
		else
		{
			// Version paramater is required
			return false;
		}

		if (!gltfMinVersion.IsNull())
		{
			asset.minVersion = gltfMinVersion.AsString();
		}
		else
		{
			asset.minVersion = asset.version;
		}

		if (!gltfGenerator.IsNull())
		{
			asset.generator = gltfGenerator.AsString();
		}
		else
		{
			asset.generator = "None";
		}

		#if GLTF_USE_EXTENTIONS

		JSONValue gltfExtensions	= gltfAsset["extensions"];
		JSONValue gltfExtras		= gltfAsset["extras"];

		if (!gltfExtensions.IsNull())
		{
			// Ignore for now
		}

		if (!gltfExtras.IsNull())
		{
			// Ignore for now
		}

		#endif

		return true;
	}

	Bool8 ParseGLTFNodes(JSONValue& gltf, Array<GLTFNode>& nodes)
	{
		JSONValue gltfNodes = gltf["nodes"];

		if (gltfNodes.IsNull())
		{
			// Invalid GLTF file
			return false;
		}

		nodes.Resize(gltfNodes.Size());

		for (UInt32 i = 0; i < gltfNodes.Size(); i++)
		{
			JSONValue gltfNode	= gltfNodes[i];
			GLTFNode& node		= nodes[i];

			JSONValue gltfName			= gltfNode["name"];
			JSONValue gltfCamera		= gltfNode["camera"];
			JSONValue gltfMesh			= gltfNode["mesh"];
			JSONValue gltfSkin			= gltfNode["skin"];
			JSONValue gltfWeights		= gltfNode["weights"];
			JSONValue gltfTranslation	= gltfNode["translation"];
			JSONValue gltfRotation		= gltfNode["rotation"];
			JSONValue gltfScale			= gltfNode["scale"];
			JSONValue gltfMatrix		= gltfNode["matrix"];
			JSONValue gltfChildren		= gltfNode["children"];

			if (!gltfName.IsNull())
			{
				node.name = gltfName.AsString();
			}
			else
			{
				char indexStr[8]{};
				_itoa_s(i, indexStr, 10);
				node.name = String("Node_") + indexStr;
			}

			if (!gltfCamera.IsNull())
			{
				node.camera = gltfCamera.AsInt();
			}
			else
			{
				node.camera = GLTF_NO_CAMERA;
			}

			if (!gltfMesh.IsNull())
			{
				node.mesh = gltfMesh.AsInt();
			}
			else
			{
				node.mesh = GLTF_NO_MESH;
			}

			if (!gltfSkin.IsNull())
			{
				node.skin = gltfSkin.AsInt();
			}
			else
			{
				node.skin = GLTF_NO_SKIN;
			}

			node.weights.Resize(gltfWeights.Size());

			for (UInt32 j = 0; j < gltfWeights.Size(); j++)
			{
				JSONValue gltfWeight = gltfWeights[j];
				node.weights[j] = gltfWeight.AsFloat();
			}

			if (!gltfTranslation.IsNull())
			{
				node.translation.x = gltfTranslation.AsArray()[0].AsFloat();
				node.translation.y = gltfTranslation.AsArray()[1].AsFloat();
				node.translation.z = gltfTranslation.AsArray()[2].AsFloat();
			}
			else
			{
				node.translation = { 0.0f, 0.0f, 0.0f };
			}

			if (!gltfRotation.IsNull())
			{
				node.rotation.x = gltfRotation.AsArray()[0].AsFloat();
				node.rotation.y = gltfRotation.AsArray()[1].AsFloat();
				node.rotation.z = gltfRotation.AsArray()[2].AsFloat();
			}
			else
			{
				node.rotation = { 0.0f, 0.0f, 0.0f };
			}

			if (!gltfScale.IsNull())
			{
				node.scale.x = gltfScale.AsArray()[0].AsFloat();
				node.scale.y = gltfScale.AsArray()[1].AsFloat();
				node.scale.z = gltfScale.AsArray()[2].AsFloat();
			}
			else
			{
				node.scale = { 1.0f, 1.0f, 1.0f };
			}

			if (!gltfMatrix.IsNull())
			{
				node.translation.x = gltfMatrix.AsArray()[3].AsFloat();
				node.translation.y = gltfMatrix.AsArray()[7].AsFloat();
				node.translation.z = gltfMatrix.AsArray()[11].AsFloat();

				node.scale.x = Math::Magnitude(
					gltfMatrix.AsArray()[0].AsFloat(),
					gltfMatrix.AsArray()[4].AsFloat(),
					gltfMatrix.AsArray()[8].AsFloat());

				node.scale.y = Math::Magnitude(
					gltfMatrix.AsArray()[1].AsFloat(),
					gltfMatrix.AsArray()[5].AsFloat(),
					gltfMatrix.AsArray()[9].AsFloat());

				node.scale.z = Math::Magnitude(
					gltfMatrix.AsArray()[2].AsFloat(),
					gltfMatrix.AsArray()[6].AsFloat(),
					gltfMatrix.AsArray()[10].AsFloat());

				Matrix4 rotMatrix
				(
					gltfMatrix.AsArray()[0].AsFloat(),
					gltfMatrix.AsArray()[1].AsFloat(),
					gltfMatrix.AsArray()[2].AsFloat(),
					0.0f,
					gltfMatrix.AsArray()[4].AsFloat(),
					gltfMatrix.AsArray()[5].AsFloat(),
					gltfMatrix.AsArray()[6].AsFloat(),
					0.0f,
					gltfMatrix.AsArray()[8].AsFloat(),
					gltfMatrix.AsArray()[9].AsFloat(),
					gltfMatrix.AsArray()[10].AsFloat(),
					0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				);

				rotMatrix = rotMatrix.DivideColumns(
					Vector4(node.scale.x),
					Vector4(node.scale.y),
					Vector4(node.scale.z),
					Vector4(1.0f));

				node.rotation = Vector3(0.0f, 0.0f, 1.0f) * Quaternion(rotMatrix);
			}

			node.children.Resize(gltfChildren.Size());

			for (UInt32 j = 0; j < gltfChildren.Size(); j++)
			{
				JSONValue gltfChild = gltfChildren[j];
				node.children[j] = gltfChild.AsInt();
			}

			#if GLTF_USE_EXTENTIONS

			JSONValue gltfExtensions	= gltfNode["extensions"];
			JSONValue gltfExtras		= gltfNode["extras"];

			if (!gltfExtensions.IsNull())
			{
				// Ignore for now
			}

			if (!gltfExtras.IsNull())
			{
				// Ignore for now
			}

			#endif
		}
	}

	Bool8 ParseGLTFScenes(JSONValue& gltf, GLTFScenes& scenes)
	{
		JSONValue gltfScenes = gltf["scenes"];

		scenes.scenes.Resize(gltfScenes.Size());

		for (UInt32 i = 0; i < gltfScenes.Size(); i++)
		{
			JSONValue	gltfScene	= gltfScenes[i];
			GLTFScene&	scene		= scenes.scenes[i];

			JSONValue gltfName	= gltfScene["name"];
			JSONValue gltfNodes	= gltfScene["nodes"];

			if (!gltfName.IsNull())
			{
				scene.name = gltfName.AsString();
			}
			else
			{
				char indexStr[8]{};
				_itoa_s(i, indexStr, 10);
				scene.name = String("Scene_") + indexStr;
			}

			scene.rootNodes.Resize(gltfNodes.Size());

			for (UInt32 j = 0; j < gltfNodes.Size(); j++)
			{
				scene.rootNodes[j] = gltfNodes[j].AsInt();
			}
		}

		return true;
	}

	Bool8 ParseGLTFScene(JSONValue& gltf, UInt32& scene)
	{
		JSONValue gltfScene = gltf["scene"];

		if (!gltfScene.IsNull())
		{
			scene = gltfScene.AsInt();
		}
		else
		{
			scene = GLTF_NO_SCENE;
		}

		return true;
	}

	Bool8 ParseGLTFCamera(JSONValue& gltf, GLTFCamera& camera)
	{
		// Ignore for now
		return true;
	}

	Bool8 ParseGLTFAccessors(JSONValue& gltf, Array<GLTFAccessor>& accessors)
	{
		JSONValue gltfAccessors = gltf["accessors"];

		accessors.Resize(gltfAccessors.Size());

		for (UInt32 i = 0; i < gltfAccessors.Size(); i++)
		{
			JSONValue		gltfAccessor	= gltfAccessors[i];
			GLTFAccessor&	accessor		= accessors[i];

			JSONValue gltfBufferView	= gltfAccessor["bufferView"];
			JSONValue gltfByteOffset	= gltfAccessor["byteOffset"];
			JSONValue gltfComponentType	= gltfAccessor["componentType"];
			JSONValue gltfNormalized	= gltfAccessor["normalized"];
			JSONValue gltfCount			= gltfAccessor["count"];
			JSONValue gltfType			= gltfAccessor["type"];
			JSONValue gltfMin			= gltfAccessor["min"];
			JSONValue gltfMax			= gltfAccessor["max"];

			// Sparse not supported

			if (!gltfBufferView.IsNull())
			{
				accessor.bufferView = gltfBufferView.AsInt();
			}
			else
			{
				// Non-bufferView access not supported
				return false;
			}

			if (!gltfByteOffset.IsNull())
			{
				accessor.byteOffset = gltfByteOffset.AsInt();
			}
			else
			{
				accessor.byteOffset = 0;
			}

			if (!gltfComponentType.IsNull())
			{
				accessor.componentType = (GLTFComponent)gltfComponentType.AsInt();
			}
			else
			{
				// Component type required
				return false;
			}

			if (!gltfNormalized.IsNull())
			{
				accessor.normalized = gltfNormalized.AsBool();
			}
			else
			{
				accessor.normalized = false;
			}

			if (!gltfCount.IsNull())
			{
				accessor.count = gltfCount.AsInt();
			}
			else
			{
				// Count required
				return false;
			}

			if (!gltfType.IsNull())
			{
				String typeStr = gltfType.AsString();

				if (typeStr == "SCALAR")
				{
					accessor.type = GLTF_TYPE_SCALAR;

					accessor.min.Resize(1, NEGATIVE_INFINITY);
					accessor.max.Resize(1, INFINITY);
				}

				if (typeStr == "VEC2")
				{
					accessor.type = GLTF_TYPE_VEC2;

					accessor.min.Resize(2, NEGATIVE_INFINITY);
					accessor.max.Resize(2, INFINITY);
				}

				if (typeStr == "VEC3")
				{
					accessor.type = GLTF_TYPE_VEC3;

					accessor.min.Resize(3, NEGATIVE_INFINITY);
					accessor.max.Resize(3, INFINITY);
				}

				if (typeStr == "VEC4")
				{
					accessor.type = GLTF_TYPE_VEC4;

					accessor.min.Resize(4, NEGATIVE_INFINITY);
					accessor.max.Resize(4, INFINITY);
				}

				if (typeStr == "MAT2")
				{
					accessor.type = GLTF_TYPE_MAT2;

					accessor.min.Resize(4, NEGATIVE_INFINITY);
					accessor.max.Resize(4, INFINITY);
				}

				if (typeStr == "MAT3")
				{
					accessor.type = GLTF_TYPE_MAT3;

					accessor.min.Resize(9, NEGATIVE_INFINITY);
					accessor.max.Resize(9, INFINITY);
				}

				if (typeStr == "MAT4")
				{
					accessor.type = GLTF_TYPE_MAT4;

					accessor.min.Resize(16, NEGATIVE_INFINITY);
					accessor.max.Resize(16, INFINITY);
				}
			}
			else
			{
				// Type required
				return false;
			}

			if (!gltfMin.IsNull())
			{
				for (UInt32 j = 0; j < gltfMin.Size(); j++)
				{
					accessor.min[j] = gltfMin[j].AsFloat();
				}
			}

			if (!gltfMax.IsNull())
			{
				for (UInt32 j = 0; j < gltfMax.Size(); j++)
				{
					accessor.max[j] = gltfMax[j].AsFloat();
				}
			}

			#if GLTF_USE_EXTENTIONS

			JSONValue gltfExtensions	= gltfAccessor["extensions"];
			JSONValue gltfExtras		= gltfAccessor["extras"];

			if (!gltfExtensions.IsNull())
			{
				// Ignore for now
			}

			if (!gltfExtras.IsNull())
			{
				// Ignore for now
			}

			#endif
		}
	}

	Bool8 ParseGLTFBuffers(JSONValue& gltf, Array<GLTFBuffer>& buffers)
	{
		JSONValue gltfBuffers = gltf["buffers"];

		buffers.Resize(gltfBuffers.Size());

		for (UInt32 i = 0; i < gltfBuffers.Size(); i++)
		{
			JSONValue	gltfBuffer	= gltfBuffers[i];
			GLTFBuffer& buffer		= buffers[i];

			JSONValue gltfName			= gltfBuffer["name"];
			JSONValue gltfURI			= gltfBuffer["uri"];
			JSONValue gltfByteLength	= gltfBuffer["byteLength"];

			if (!gltfName.IsNull())
			{
				buffer.name = gltfName.AsString();
			}
			else
			{
				char indexStr[8]{};
				_itoa_s(i, indexStr, 10);
				buffer.name = String("Buffer_") + indexStr;
			}

			if (!gltfURI.IsNull())
			{
				buffer.uri = gltfURI.AsString();
			}
			else
			{
				// Buffer must have a path to the data
				return false;
			}

			if (!gltfByteLength.IsNull())
			{
				buffer.byteLength = gltfByteLength.AsInt();
			}
			else
			{
				// Buffer must have a length
				return false;
			}

			#if GLTF_USE_EXTENTIONS

			JSONValue gltfExtensions	= gltfBuffer["extensions"];
			JSONValue gltfExtras		= gltfBuffer["extras"];

			if (!gltfExtensions.IsNull())
			{
				// Ignore for now
			}

			if (!gltfExtras.IsNull())
			{
				// Ignore for now
			}

			#endif
		}

		return true;
	}

	Bool8 ParseGLTFBufferViews(JSONValue& gltf, Array<GLTFBufferView>& bufferViews)
	{
		JSONValue gltfBufferViews = gltf["bufferViews"];

		bufferViews.Resize(gltfBufferViews.Size());

		for (UInt32 i = 0; i < gltfBufferViews.Size(); i++)
		{
			JSONValue		gltfBufferView	= gltfBufferViews[i];
			GLTFBufferView& bufferView		= bufferViews[i];

			JSONValue gltfName			= gltfBufferView["name"];
			JSONValue gltfBuffer		= gltfBufferView["buffer"];
			JSONValue gltfByteLength	= gltfBufferView["byteLength"];
			JSONValue gltfByteStride	= gltfBufferView["byteStride"];
			JSONValue gltfTarget		= gltfBufferView["target"];

			if (!gltfName.IsNull())
			{
				bufferView.name = gltfName.AsString();
			}
			else
			{
				char indexStr[8]{};
				_itoa_s(i, indexStr, 10);
				bufferView.name = String("BufferView_") + indexStr;
			}

			if (!gltfBuffer.IsNull())
			{
				bufferView.buffer = gltfBuffer.AsInt();
			}
			else
			{
				// BufferView requires a buffer id
				return false;
			}

			if (!gltfByteLength.IsNull())
			{
				bufferView.byteLength = gltfByteLength.AsInt();
			}
			else
			{
				// BufferView requires a byte length
				return false;
			}

			if (!gltfByteStride.IsNull())
			{
				//TODO: Ensure multiple of 4?

				bufferView.byteStride = gltfByteStride.AsInt();
			}
			else
			{
				bufferView.byteStride = 4;
			}

			if (!gltfTarget.IsNull())
			{
				bufferView.target = (GLTFTarget)gltfTarget.AsInt();
			}
			else
			{
				bufferView.target = GLTF_TARGET_ARRAY_BUFFER;
			}

			#if GLTF_USE_EXTENTIONS

			JSONValue gltfExtensions	= gltfBufferView["extensions"];
			JSONValue gltfExtras		= gltfBufferView["extras"];

			if (!gltfExtensions.IsNull())
			{
				// Ignore for now
			}

			if (!gltfExtras.IsNull())
			{
				// Ignore for now
			}

			#endif
		}

		return true;
	}

	Bool8 ParseGLTFMeshes(JSONValue& gltf, Array<GLTFMesh>& meshes)
	{
		JSONValue gltfMeshes = gltf["meshes"];

		meshes.Resize(gltfMeshes.Size());

		for (UInt32 i = 0; i < gltfMeshes.Size(); i++)
		{
			JSONValue gltfMesh	= gltfMeshes[i];
			GLTFMesh& mesh		= meshes[i];

			JSONValue gltfName			= gltfMesh["name"];
			JSONValue gltfPrimitives	= gltfMesh["primitives"];
			JSONValue gltfWeights		= gltfMesh["weights"];

			if (!gltfName.IsNull())
			{
				mesh.name = gltfName.AsString();
			}
			else
			{
				char indexStr[8]{};
				_itoa_s(i, indexStr, 10);
				mesh.name = String("Mesh_") + indexStr;
			}

			if (gltfPrimitives.IsNull() || gltfPrimitives.Size() == 0)
			{
				// Must have at least one primitive
				return false;
			}

			mesh.primitives.Resize(gltfPrimitives.Size());

			for (UInt32 j = 0; j < gltfPrimitives.Size(); j++)
			{
				JSONValue		gltfPrimitive	= gltfPrimitives[j];
				GLTFPrimitive&	primitive		= mesh.primitives[j];

				JSONValue gltfAttributes		= gltfPrimitive["attributes"];
				JSONValue gltfIndices			= gltfPrimitive["indices"];
				JSONValue gltfMaterial			= gltfPrimitive["material"];
				JSONValue gltfMode				= gltfPrimitive["mode"];
				JSONValue gltfTargets			= gltfPrimitive["targets"];

				if (gltfAttributes.IsNull())
				{
					// Must contain at least one attribute
					return false;
				}
				
				JSONValue gltfAttribPOSITION	= gltfAttributes["POSITION"];
				JSONValue gltfAttribNORMAL		= gltfAttributes["NORMAL"];
				JSONValue gltfAttribTANGENT		= gltfAttributes["TANGENT"];
				JSONValue gltfAttribTEXCOORD_0	= gltfAttributes["TEXCOORD_0"];
				JSONValue gltfAttribTEXCOORD_1	= gltfAttributes["TEXCOORD_1"];
				JSONValue gltfAttribCOLOR_0		= gltfAttributes["COLOR_0"];
				JSONValue gltfAttribJOINTS_0	= gltfAttributes["JOINTS_0"];
				JSONValue gltfAttribWEIGHTS_0	= gltfAttributes["WEIGHTS_0"];

				if (!gltfAttribPOSITION.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_POSITION_BIT;
					primitive.attributes[0] = gltfAttribPOSITION.AsInt();
				}
				else
				{
					// Must contain at least position data
					return false;
				}

				if (!gltfAttribNORMAL.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_NORMAL_BIT;
					primitive.attributes[1] = gltfAttribNORMAL.AsInt();
				}

				if (!gltfAttribTANGENT.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_TANGENT_BIT;
					primitive.attributes[2] = gltfAttribTANGENT.AsInt();
				}

				if (!gltfAttribTEXCOORD_0.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_TEXCOORD_0_BIT;
					primitive.attributes[3] = gltfAttribTEXCOORD_0.AsInt();
				}

				if (!gltfAttribTEXCOORD_1.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_TEXCOORD_1_BIT;
					primitive.attributes[4] = gltfAttribTEXCOORD_1.AsInt();
				}

				if (!gltfAttribCOLOR_0.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_COLOR_0_BIT;
					primitive.attributes[5] = gltfAttribCOLOR_0.AsInt();
				}

				if (!gltfAttribJOINTS_0.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_JOINTS_0_BIT;
					primitive.attributes[6] = gltfAttribJOINTS_0.AsInt();
				}

				if (!gltfAttribWEIGHTS_0.IsNull())
				{
					primitive.attributeFlags |= GLTF_ATTRIBUTE_WEIGHTS_0_BIT;
					primitive.attributes[7] = gltfAttribWEIGHTS_0.AsInt();
				}

				if (!gltfIndices.IsNull())
				{
					primitive.indices = gltfIndices.AsInt();
				}
				else
				{
					primitive.indices = GLTF_NO_INDICES;
				}

				if (!gltfMaterial.IsNull())
				{
					primitive.material = gltfMaterial.AsInt();
				}
				else
				{
					primitive.indices = GLTF_NO_MATERIAL;
				}

				if (!gltfMode.IsNull())
				{
					primitive.mode = (GLTFPrimitiveMode)gltfMode.AsInt();
				}
				else
				{
					primitive.mode = GLTF_PRIMITIVE_MODE_TRIANGLES;
				}

				if (!gltfTargets.IsNull())
				{
					if (gltfTargets.Size() == 0)
					{
						// Must contain at least one target
						return false;
					}

					JSONValue gltfTargetPOSITION	= gltfTargets["POSITION"];
					JSONValue gltfTargetNORMAL		= gltfTargets["NORMAL"];
					JSONValue gltfTargetTANGENT		= gltfTargets["TANGENT"];
					JSONValue gltfTargetTEXCOORD_0	= gltfTargets["TEXCOORD_0"];
					JSONValue gltfTargetTEXCOORD_1	= gltfTargets["TEXCOORD_1"];
					JSONValue gltfTargetCOLOR_0		= gltfTargets["COLOR_0"];
					JSONValue gltfTargetJOINTS_0	= gltfTargets["JOINTS_0"];
					JSONValue gltfTargetWEIGHTS_0	= gltfTargets["WEIGHTS_0"];

					if (!gltfTargetPOSITION.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_POSITION_BIT;
						primitive.morphTargets[0] = gltfTargetPOSITION.AsInt();
					}

					if (!gltfTargetNORMAL.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_NORMAL_BIT;
						primitive.morphTargets[1] = gltfTargetNORMAL.AsInt();
					}

					if (!gltfTargetTANGENT.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_TANGENT_BIT;
						primitive.morphTargets[2] = gltfTargetTANGENT.AsInt();
					}

					if (!gltfTargetTEXCOORD_0.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_TEXCOORD_0_BIT;
						primitive.morphTargets[3] = gltfTargetTEXCOORD_0.AsInt();
					}

					if (!gltfTargetTEXCOORD_1.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_TEXCOORD_1_BIT;
						primitive.morphTargets[4] = gltfTargetTEXCOORD_1.AsInt();
					}

					if (!gltfTargetCOLOR_0.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_COLOR_0_BIT;
						primitive.morphTargets[5] = gltfTargetCOLOR_0.AsInt();
					}

					if (!gltfTargetJOINTS_0.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_JOINTS_0_BIT;
						primitive.morphTargets[6] = gltfTargetJOINTS_0.AsInt();
					}

					if (!gltfTargetWEIGHTS_0.IsNull())
					{
						primitive.morphTargetFlags |= GLTF_ATTRIBUTE_WEIGHTS_0_BIT;
						primitive.morphTargets[7] = gltfTargetWEIGHTS_0.AsInt();
					}
				}

				#if GLTF_USE_EXTENTIONS

				JSONValue gltfExtensions	= gltfPrimitive["extensions"];
				JSONValue gltfExtras		= gltfPrimitive["extras"];

				if (!gltfExtensions.IsNull())
				{
					// Ignore for now
				}

				if (!gltfExtras.IsNull())
				{
					// Ignore for now
				}

				#endif
			}

			#if GLTF_USE_EXTENTIONS

			JSONValue gltfExtensions	= gltfMesh["extensions"];
			JSONValue gltfExtras		= gltfMesh["extras"];

			if (!gltfExtensions.IsNull())
			{
				// Ignore for now
			}

			if (!gltfExtras.IsNull())
			{
				// Ignore for now
			}

			#endif
		}

		return true;
	}

    Bool8 LoadGLTF(JSON& gltfJson, GLTFModel& gltfObject)
    {
		JSONValue& gltf = *gltfJson.GetRoot();
		
		

		

		JSONValue gltfTextures		= gltf["textures"];
		JSONValue gltfImages		= gltf["images"];
		JSONValue gltfSamplers		= gltf["samplers"];
		
		

		JSONValue gltfMaterials		= gltf["materials"];
		JSONValue gltfAnimations	= gltf["animations"];
		JSONValue gltfSkins			= gltf["skins"];

		ParseGLTFAsset(gltf, gltfObject.asset);
		ParseGLTFNodes(gltf, gltfObject.nodes);
		ParseGLTFScenes(gltf, gltfObject.scenes);
		ParseGLTFScene(gltf, gltfObject.scene);
		ParseGLTFCamera(gltf, gltfObject.camera);
		ParseGLTFAccessors(gltf, gltfObject.accessors);
		ParseGLTFBuffers(gltf, gltfObject.buffers);
		ParseGLTFBufferViews(gltf, gltfObject.bufferViews);
		ParseGLTFMeshes(gltf, gltfObject.meshes);

        return true;
    }
}

