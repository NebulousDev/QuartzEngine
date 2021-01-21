#pragma once

#include "vulkan/vulkan.h"
#include "spirv-headers/spirv.h"

#include "util/Array.h"
#include "util/String.h"

namespace Quartz
{
	enum SpirvType
	{
		SPIRV_TYPE_UNKNOWN,
		SPIRV_TYPE_VOID,
		SPIRV_TYPE_BOOL,
		SPIRV_TYPE_INT,
		SPIRV_TYPE_FLOAT,
		SPIRV_TYPE_VECTOR,
		SPIRV_TYPE_MATRIX,
		SPIRV_TYPE_IMAGE,
		SPIRV_TYPE_SAMPLER,
		SPIRV_TYPE_SAMPLED_IMAGE,
		SPIRV_TYPE_ARRAY,
		SPIRV_TYPE_RUNTIME_ARRAY,
		SPIRV_TYPE_STRUCT,
		SPIRV_TYPE_OPAQUE,
		SPIRV_TYPE_POINTER,
		SPIRV_TYPE_FUNCTION,
		SPIRV_TYPE_EVENT,			// <VVV not implemented
		SPIRV_TYPE_DEVICE_EVENT,
		SPIRV_TYPE_RESERVE_ID,
		SPIRV_TYPE_QUEUE,
		SPIRV_TYPE_PIPE,
		SPIRV_TYPE_FORWARD_POINTER,
		SPIRV_TYPE_PIPE_STORAGE,
		SPIRV_TYPE_NAMED_BARRIER,
		SPIRV_TYPE_RAY_QUERY_PROVISIONAL,
		SPIRV_TYPE_ACCELERATION_STRUCTURE,
		SPIRV_TYPE_COOPERATIVE_MATRIX
	};

	enum SpirvImageDimension
	{
		SPIRV_IMAGE_1D,
		SPIRV_IMAGE_2D,
		SPIRV_IMAGE_3D,
		SPIRV_IMAGE_CUBE,
		SPIRV_IMAGE_RECT,
		SPIRV_IMAGE_BUFFER,
		SPIRV_IMAGE_SUBPASS_DATA
	};

	struct SpirvDecoration
	{
		UInt32	set;
		UInt32	location;
		UInt32	binding;
		Bool8	block;
		Bool8	bufferBlock;
	};

	struct SpirvIntType
	{
		UInt32 width;
		UInt32 signedness;
	};

	struct SpirvFloatType
	{
		UInt32 width;
	};

	struct SpirvVectorType
	{
		UInt32 typeId;
		UInt32 count;
	};

	struct SpirvMatrixType
	{
		UInt32 typeId;
		UInt32 columnCount;
	};

	struct SpirvImageType
	{
		UInt32 sampledTypeId;
		SpvDim dimension;
		UInt32 depth;
		UInt32 arrayed;
		UInt32 multisampled;
		UInt32 sampled;
		SpvImageFormat imageFormat;
		SpvAccessQualifier access;
	};

	struct SpirvSampledImage
	{
		UInt32 imageTypeId;
	};

	struct SpirvArrayType
	{
		UInt32 typeId;
		UInt32 length;
	};

	struct SpirvRuntimeArrayType
	{
		UInt32 typeId;
	};

#define SPIRV_MAX_STRUCT_SIZE 16

	struct SprivStructType
	{
		UInt32 structEntryIds[SPIRV_MAX_STRUCT_SIZE];
	};

	struct SpirvOpaqueType
	{
		String name;
	};

	struct SprivPointerType
	{
		SpvStorageClass storageClass;
		UInt32 typeId;
	};

#define SPIRV_MAX_FUNCTION_PARAMETER_SIZE 15

	struct SpirvFunctionType
	{
		UInt32 returnTypeId;
		UInt32 paramaterTypeIds[15];
	};

	struct SpirvObject
	{
		String			name;
		SpirvType		type;
		UInt32			variableId;
		SpvStorageClass	storageClass;
		SpirvDecoration	decoration;
		Bool8			isMember;

		// To prevent default objects having the
		// SpvStorageClassUniformConstant (0) storage class
		FORCE_INLINE SpirvObject() :
			name(),
			type(SPIRV_TYPE_UNKNOWN),
			variableId(-1),
			storageClass(SpvStorageClass(-1)),
			decoration{},
			isMember(false),
			structType{},
			opaqueType{}
		{}

		union
		{
			SpirvIntType			intType;
			SpirvFloatType			floatType;
			SpirvVectorType			vectorType;
			SpirvMatrixType			matrixType;
			SpirvImageType			imageType;
			SpirvSampledImage		sampledImageType;
			SpirvArrayType			arrayType;
			SpirvRuntimeArrayType	runtimeArrayType;
			SprivStructType			structType;
			SprivPointerType		pointerType;
			SpirvFunctionType		functionType;
		};

		// Since String can't be used in a union, SpirvOpaqueType 
		// must be separate from the union
		SpirvOpaqueType	opaqueType;
	};

	struct SpirvReflection
	{
		String entryName;
		SpvExecutionModel executionModel;
		VkShaderStageFlags shaderStage;
		Array<SpirvObject> objects;
	};

	struct SpirvUniform
	{
		String name;
		UInt32 set;
		UInt32 binding;
		Bool8 isBlock;
		UInt32 sizeBytes;
		VkDescriptorType descriptorType;
		VkShaderStageFlags shaderStage;
	};

	struct SpirvAttribute
	{
		String name;
		UInt32 location;
		UInt32 binding;
		VkFormat format;
	};

	Bool8 SpirvParseReflection(SpirvReflection* pReflection, const Array<Byte>& spirvCode);

	void SpirvExtractUniforms(Array<SpirvUniform>& uniforms, SpirvReflection& reflection);

	void SpirvExtractAttributes(Array<SpirvAttribute>& attributes, SpirvReflection& reflection);

	UInt32 SpirvObjectSize(const SpirvObject& object, SpirvReflection& reflection);

	VkShaderStageFlagBits SpvExecutionModelToVkShaderStageFlags(SpvExecutionModel executionModel);
}