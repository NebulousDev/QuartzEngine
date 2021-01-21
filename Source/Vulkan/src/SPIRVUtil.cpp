#include "SPIRVUtil.h"

#include "util/Array.h"
#include "spirv-headers/spirv.h"

#include <cstdio>

namespace Quartz
{
	// Loosly based off spirv parsing by Arseny Kapoulkine shown here:
	// https://www.youtube.com/watch?v=3Py4GlWAicY
	Bool8 SpirvParseReflection(SpirvReflection* pReflection, const Array<Byte>& spirvCode)
	{
		const UInt32* pCode = reinterpret_cast<const UInt32*>(spirvCode.Data());
		const UInt32 codeSizeInts = spirvCode.Size() / 4;

		if (*pCode != SpvMagicNumber)
		{
			//Not a valid spirv file
			return false;
		}

		Array<SpirvObject>& objects = pReflection->objects; 

		UInt32 maxId = pCode[3];
		objects.Resize(maxId);

		// Pass 1:

		// Advance to start of instructions
		const UInt32* inst = pCode + 5;

		while (inst != pCode + codeSizeInts)
		{
			UInt16 opCode = UInt16(inst[0]);
			UInt16 wordCount = UInt16(inst[0] >> 16);

			switch (opCode)
			{
				// OpType operations:

				case SpvOpTypeVoid:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_VOID;
					break;
				}

				case SpvOpTypeBool:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_BOOL;
					break;
				}

				case SpvOpTypeInt:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_INT;
					objects[id].intType.width = inst[2];
					objects[id].intType.signedness = inst[3];
					break;
				}

				case SpvOpTypeFloat:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_FLOAT;
					objects[id].floatType.width = inst[2];
					break;
				}

				case SpvOpTypeVector:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_VECTOR;
					objects[id].vectorType.typeId = inst[2];
					objects[id].vectorType.count = inst[3];
					break;
				}

				case SpvOpTypeMatrix:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_MATRIX;
					objects[id].matrixType.typeId = inst[2];
					objects[id].matrixType.columnCount = inst[3];
					break;
				}

				case SpvOpTypeImage:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_IMAGE;
					objects[id].imageType.sampledTypeId = inst[2];
					objects[id].imageType.dimension = SpvDim(inst[3]);
					objects[id].imageType.depth = inst[4];
					objects[id].imageType.arrayed = inst[5];
					objects[id].imageType.multisampled = inst[5];
					objects[id].imageType.sampled = inst[6];
					objects[id].imageType.imageFormat = SpvImageFormat(inst[7]);
					objects[id].imageType.access = SpvAccessQualifier(inst[8]);
					break;
				}

				case SpvOpTypeSampler:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_SAMPLER;
					break;
				}

				case SpvOpTypeSampledImage:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_SAMPLED_IMAGE;
					objects[id].sampledImageType.imageTypeId = inst[2];
					break;
				}

				case SpvOpTypeArray:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_ARRAY;
					objects[id].arrayType.typeId = inst[2];
					objects[id].arrayType.length = inst[3];
					break;
				}

				case SpvOpTypeRuntimeArray:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_RUNTIME_ARRAY;
					objects[id].runtimeArrayType.typeId = inst[2];
					break;
				}

				case SpvOpTypeStruct:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_STRUCT;

					if (wordCount > SPIRV_MAX_STRUCT_SIZE)
					{
						return false;
					}

					for (UInt32 i = 2; i < wordCount; i++)
					{
						UInt32 memberId = inst[i];
						objects[id].structType.structEntryIds[i - 2] = memberId;
						objects[memberId].isMember = true;
					}

					break;
				}

				case SpvOpTypeOpaque:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_OPAQUE;
					const char* name = (const char*)(inst + 2);
					objects[id].opaqueType.name = String(name);
					break;
				}

				case SpvOpTypePointer:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_POINTER;
					objects[id].pointerType.storageClass = SpvStorageClass(inst[2]);
					objects[id].pointerType.typeId = inst[3];
					break;
				}

				case SpvOpTypeFunction:
				{
					UInt32 id = inst[1];
					objects[id].type = SPIRV_TYPE_FUNCTION;
					objects[id].functionType.returnTypeId = inst[2];

					if (wordCount > SPIRV_MAX_FUNCTION_PARAMETER_SIZE)
					{
						return false;
					}

					for (UInt32 i = 3; i < wordCount; i++)
					{
						objects[id].functionType.paramaterTypeIds[i - 3] = inst[i];
					}

					break;
				}

				// Other

				case SpvOpEntryPoint:
				{
					pReflection->executionModel = SpvExecutionModel(inst[1]);
					pReflection->shaderStage = SpvExecutionModelToVkShaderStageFlags(pReflection->executionModel);
					const char* entryName = (const char*)(inst + 3);
					pReflection->entryName = String(entryName);
					break;
				}

				case SpvOpDecorate:
				{
					UInt32 id = inst[1];
					SpvDecoration type = SpvDecoration(inst[2]);

					switch (type)
					{
						case SpvDecorationBlock:
						{
							objects[id].decoration.block = true;
							break;
						}

						case SpvDecorationBufferBlock:
						{
							objects[id].decoration.bufferBlock = true;
							break;
						}

						case SpvDecorationDescriptorSet:
						{
							objects[id].decoration.set = inst[3];
							break;
						}

						case SpvDecorationBinding:
						{
							objects[id].decoration.binding = inst[3];
							break;
						}

						case SpvDecorationLocation:
						{
							objects[id].decoration.location = inst[3];
							break;
						}

						default:
							break;
					}

					break;
				}

				case SpvOpVariable:
				{
					UInt32 id = inst[2];
					objects[id].variableId = inst[1];
					objects[id].storageClass = SpvStorageClass(inst[3]);
					break;
				}

				case SpvOpName:
				{
					UInt32 id = inst[1];
					const char* valueName = (const char*)(inst + 2);
					objects[id].name = String(valueName);
					break;
				}

				default:
					break;
			}

			inst += wordCount;
		}

		// Pass 2:

		inst = pCode + 5;

		while (inst != pCode + codeSizeInts)
		{
			UInt16 opCode = UInt16(inst[0]);
			UInt16 wordCount = UInt16(inst[0] >> 16);

			switch (opCode)
			{

				case SpvOpMemberName:
				{
					UInt32 id = inst[1];
					UInt32 memberId = objects[id].structType.structEntryIds[inst[2]];
					const char* memberName = (const char*)(inst + 3);
					objects[memberId].name = String(memberName);
					break;
				}

				default:
					break;
			}

			inst += wordCount;
		}

		return true;
	}

	VkShaderStageFlagBits SpvExecutionModelToVkShaderStageFlags(SpvExecutionModel executionModel)
	{
		switch (executionModel)
		{
		case SpvExecutionModelVertex:
			return VK_SHADER_STAGE_VERTEX_BIT;
		case SpvExecutionModelTessellationControl:
			return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case SpvExecutionModelTessellationEvaluation:
			return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case SpvExecutionModelGeometry:
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		case SpvExecutionModelFragment:
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		case SpvExecutionModelGLCompute:
			return VK_SHADER_STAGE_COMPUTE_BIT;
		case SpvExecutionModelTaskNV:
			return VK_SHADER_STAGE_TASK_BIT_NV;
		case SpvExecutionModelMeshNV:
			return VK_SHADER_STAGE_MESH_BIT_NV;
		case SpvExecutionModelRayGenerationKHR:
			return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		case SpvExecutionModelIntersectionKHR:
			return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
		case SpvExecutionModelAnyHitKHR:
			return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
		case SpvExecutionModelClosestHitKHR:
			return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		case SpvExecutionModelMissKHR:
			return VK_SHADER_STAGE_MISS_BIT_KHR;
		case SpvExecutionModelCallableKHR:
			return VK_SHADER_STAGE_CALLABLE_BIT_KHR;

		default:
			VkShaderStageFlagBits(-1); // Unknown
		}
	}

	// @Unfinished: because I realized attribute formats must still be set on C++ side, and cant be
	//	inferred from Spirv dierectly. I may revisit this in the future with special GLSL declarations
	static VkFormat FindSpirvObjectVkFormat(const SpirvObject& object, const SpirvReflection& reflection)
	{
		switch (object.type)
		{
			case SPIRV_TYPE_INT:
			{
				if (object.intType.signedness)
				{
					switch (object.intType.width)
					{
					case 8:
						return VK_FORMAT_R8_SINT;
					case 16:
						return VK_FORMAT_R16_SINT;
					case 32:
						return VK_FORMAT_R32_SINT;
					case 64:
						return VK_FORMAT_R64_SINT;

					default:
						return VK_FORMAT_UNDEFINED;
					}
				}
				else
				{
					switch (object.intType.width)
					{
						case 8:
							return VK_FORMAT_R8_UINT;
						case 16:
							return VK_FORMAT_R16_UINT;
						case 32:
							return VK_FORMAT_R32_UINT;
						case 64:
							return VK_FORMAT_R64_UINT;

						default:
							return VK_FORMAT_UNDEFINED;
					}
				}

				return VK_FORMAT_UNDEFINED;
			}
			case SPIRV_TYPE_FLOAT:
			{
				switch (object.floatType.width)
				{
				case 16:
					return VK_FORMAT_R16_SFLOAT;
				case 32:
					return VK_FORMAT_R32_SFLOAT;
				case 64:
					return VK_FORMAT_R64_SFLOAT;

				default:
					return VK_FORMAT_UNDEFINED;
				}

				return VK_FORMAT_UNDEFINED;
			}
			case SPIRV_TYPE_VECTOR:
			{
				const SpirvObject& typeObject = reflection.objects[object.vectorType.typeId];

				switch (typeObject.type)
				{
					case SPIRV_TYPE_INT:
					{
						if (typeObject.intType.signedness)
						{
							switch (object.vectorType.count)
							{
								case 2:
									return VK_FORMAT_R32G32_SINT;
								case 3:
									return VK_FORMAT_R32G32B32_SINT;
								case 4:
									return VK_FORMAT_R32G32B32A32_SINT;

								default:
									return VK_FORMAT_UNDEFINED;
							}
						}
						else
						{
							switch (object.vectorType.count)
							{
								case 2:
									return VK_FORMAT_R32G32_UINT;
								case 3:
									return VK_FORMAT_R32G32B32_UINT;
								case 4:
									return VK_FORMAT_R32G32B32A32_UINT;

								default:
									return VK_FORMAT_UNDEFINED;
							}
						}
					}

					case SPIRV_TYPE_FLOAT:
					{
						switch (object.vectorType.count)
						{
						case 2:
							return VK_FORMAT_R32G32_SFLOAT;
						case 3:
							return VK_FORMAT_R32G32B32_SFLOAT;
						case 4:
							return VK_FORMAT_R32G32B32A32_SFLOAT;

						default:
							return VK_FORMAT_UNDEFINED;
						}	
					}

					default:
						VK_FORMAT_UNDEFINED;
				}

				return VK_FORMAT_UNDEFINED;
			}

			case SPIRV_TYPE_MATRIX:
			{
				// @Unfinished
			}

			case SPIRV_TYPE_ARRAY:
			{
				// @Unfinished
			}

			default:
				return VK_FORMAT_UNDEFINED;
		}
	}

	void SpirvExtractUniforms(Array<SpirvUniform>& uniforms, SpirvReflection& reflection)
	{
		for (UInt32 i = 0; i < reflection.objects.Size(); i++)
		{
			SpirvObject& object = reflection.objects[i];

			if (object.storageClass == SpvStorageClassUniformConstant)
			{
				// Only sampler2D constant uniforms supported by Vulkan

				SpirvObject& pointerObject = reflection.objects[object.variableId];
				SpirvObject& valueObject = reflection.objects[pointerObject.pointerType.typeId];

				if (valueObject.type == SPIRV_TYPE_SAMPLED_IMAGE)
				{
					SpirvUniform uniform;
					uniform.name = object.name;
					uniform.set = object.decoration.set;
					uniform.binding = object.decoration.binding;
					uniform.sizeBytes = SpirvObjectSize(valueObject, reflection);
					uniform.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					uniform.shaderStage = reflection.shaderStage;

					uniforms.PushBack(uniform);
				}
			}

			else if (object.storageClass == SpvStorageClassUniform)
			{
				SpirvObject& pointerObject = reflection.objects[object.variableId];
				SpirvObject& valueObject = reflection.objects[pointerObject.pointerType.typeId];

				SpirvUniform uniform;
				uniform.name = object.name;
				uniform.set = object.decoration.set;
				uniform.binding = object.decoration.binding;
				uniform.sizeBytes = SpirvObjectSize(valueObject, reflection);
				uniform.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				uniform.shaderStage = reflection.shaderStage;

				uniforms.PushBack(uniform);
			}
		}
	}

	void SpirvExtractAttributes(Array<SpirvAttribute>& attributes, SpirvReflection& reflection)
	{
		for (UInt32 i = 0; i < reflection.objects.Size(); i++)
		{
			SpirvObject& object = reflection.objects[i];

			if (object.storageClass == SpvStorageClassInput)
			{
				SpirvObject& pointerObject = reflection.objects[object.variableId];
				SpirvObject& valueObject = reflection.objects[pointerObject.pointerType.typeId];

				SpirvAttribute attribute;
				attribute.name = object.name;
				attribute.location = object.decoration.location;
				attribute.binding = object.decoration.binding;

				attributes.PushBack(attribute);
			}
		}
	}

	UInt32 SpirvObjectSize(const SpirvObject& object, SpirvReflection& reflection)
	{
		switch (object.type)
		{
			case SPIRV_TYPE_UNKNOWN:
			{
				return 0; // undefined
			}
			case SPIRV_TYPE_VOID:
			{
				return 0; // undefined
			}
			case SPIRV_TYPE_BOOL:
			{
				return 4; // probably?
			}
			case SPIRV_TYPE_INT:
			{
				return object.intType.width / 8;
			}
			case SPIRV_TYPE_FLOAT:
			{
				return object.floatType.width / 8;
			}
			case SPIRV_TYPE_VECTOR:
			{
				SpirvObject& typeObject = reflection.objects[object.vectorType.typeId];
				UInt32 typeSize = SpirvObjectSize(typeObject, reflection);
				return typeSize * object.vectorType.count;
			}
			case SPIRV_TYPE_MATRIX:
			{
				SpirvObject& typeObject = reflection.objects[object.matrixType.typeId];
				UInt32 typeSize = SpirvObjectSize(typeObject, reflection);
				return typeSize * object.matrixType.columnCount;
			}
			case SPIRV_TYPE_IMAGE:
			{
				return 4; // probably?
			}
			case SPIRV_TYPE_SAMPLER:
			{
				return 4; // probably?
			}
			case SPIRV_TYPE_SAMPLED_IMAGE:
			{
				return 4; // probably?
			}
			case SPIRV_TYPE_ARRAY:
			{
				SpirvObject& typeObject = reflection.objects[object.arrayType.typeId];
				UInt32 typeSize = SpirvObjectSize(typeObject, reflection);
				return typeSize * object.arrayType.length;
			}
			case SPIRV_TYPE_RUNTIME_ARRAY:
			{
				return 0; // undefined
			}
			case SPIRV_TYPE_STRUCT:
			{
				// @Unfinished: This does not take into account padding!!!

				UInt32 size = 0;

				for (UInt32 i = 0; i < SPIRV_MAX_STRUCT_SIZE; i++)
				{
					UInt32 memberId = object.structType.structEntryIds[i];

					if (memberId == 0)
					{
						// No more valid struct entries
						break;
					}

					SpirvObject memberObject = reflection.objects[memberId];
					size += SpirvObjectSize(memberObject, reflection);
				}
			}
			case SPIRV_TYPE_OPAQUE:
			{
				return 0; // undefined
			}
			case SPIRV_TYPE_POINTER:
			{
				return 4; // probably?
			}
			case SPIRV_TYPE_FUNCTION:
			{
				return 0; // undefined
			}

			default:
				return 0;
		}
	}
}