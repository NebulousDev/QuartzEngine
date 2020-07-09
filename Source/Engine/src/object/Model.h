#pragma once

#include "Common.h"
#include "util\Buffer.h"

namespace Quartz
{
	enum VertexElementAttribute
	{
		VERTEX_ATTRIBUTE_UNKNOWN,
		VERTEX_ATTRIBUTE_POSITION,
		VERTEX_ATTRIBUTE_NORMAL,
		VERTEX_ATTRIBUTE_BINORMAL,
		VERTEX_ATTRIBUTE_TANGENT,
		VERTEX_ATTRIBUTE_TEXCOORD,
		VERTEX_ATTRIBUTE_COLOR,
		VERTEX_ATTRIBURE_CUSTOM
	};

	enum VertexElementType
	{
		VERTEX_TYPE_FLOAT,
		VERTEX_TYPE_FLOAT2,
		VERTEX_TYPE_FLOAT3,
		VERTEX_TYPE_FLOAT4,
		VERTEX_TYPE_INT,
		VERTEX_TYPE_INT2,
		VERTEX_TYPE_INT3,
		VERTEX_TYPE_INT4,
		VERTEX_TYPE_UINT,
		VERTEX_TYPE_UINT2,
		VERTEX_TYPE_UINT3,
		VERTEX_TYPE_UINT4,
		VERTEX_TYPE_INT_2_10_10_10,
		VERTEX_TYPE_UINT_2_10_10_10
	};

	struct VertexElement
	{
		UInt32 location;
		VertexElementAttribute attribute;
		VertexElementType type;
	};

#define VERTEX_FORMAT_MAX_ELEMENT_COUNT 8

	struct VertexFormat
	{
		VertexElement elements[VERTEX_FORMAT_MAX_ELEMENT_COUNT];
		UInt32 elementCount;
	};

	struct VertexData
	{
		VertexFormat format;
		ByteBuffer buffer;
	};

	enum IndexDataFormat
	{
		INDEX_FORMAT_INT8,
		INDEX_FORMAT_UINT8,
		INDEX_FORMAT_INT16,
		INDEX_FORMAT_UINT16,
		INDEX_FORMAT_INT32,
		INDEX_FORMAT_UINT32
	};

	struct IndexData
	{
		IndexDataFormat format;
		ByteBuffer buffer;
	};

	struct Model
	{
		VertexData vertexData;
		IndexData indexData;
	};
}