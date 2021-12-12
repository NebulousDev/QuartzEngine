#pragma once

#include "../Resource.h"

#include "util/Array.h"
#include "util/String.h"
#include "math/Math.h"

namespace Quartz
{
	#define GLTF_NO_SCENE		-1
	#define GLTF_NO_CAMERA		-1
	#define GLTF_NO_MESH		-1
	#define GLTF_NO_SKIN		-1
	#define GLTF_NO_INDICES		-1
	#define GLTF_NO_MATERIAL	-1

	#define GLTF_USE_EXTENTIONS	0

	enum GLTFComponent
	{
		GLTF_COMPONENT_BYTE				= 5120,
		GLTF_COMPONENT_UNSIGNED_BYTE	= 5121,
		GLTF_COMPONENT_SHORT			= 5122,
		GLTF_COMPONENT_UNSIGNED_SHORT	= 5123,
		GLTF_COMPONENT_UNSIGNED_INT		= 5125,
		GLTF_COMPONENT_FLOAT			= 5126,
	};

	enum GLTFType
	{
		GLTF_TYPE_SCALAR,
		GLTF_TYPE_VEC2,
		GLTF_TYPE_VEC3,
		GLTF_TYPE_VEC4,
		GLTF_TYPE_MAT2,
		GLTF_TYPE_MAT3,
		GLTF_TYPE_MAT4,
	};

	enum GLTFTarget
	{
		GLTF_TARGET_ARRAY_BUFFER			= 34962,
		GLTF_TARGET_ELEMENT_ARRAY_BUFFER	= 34963
	};

	enum GLTFPrimitiveMode
	{
		GLTF_PRIMITIVE_MODE_POINTS			= 0,
		GLTF_PRIMITIVE_MODE_LINES			= 1,
		GLTF_PRIMITIVE_MODE_LINE_LOOP		= 2,
		GLTF_PRIMITIVE_MODE_LINE_STRIP		= 3,
		GLTF_PRIMITIVE_MODE_TRIANGLES		= 4,
		GLTF_PRIMITIVE_MODE_TRIANGLE_STRIP	= 5,
		GLTF_PRIMITIVE_MODE_TRIANGLE_FAN	= 6
	};

	enum GLTFAttribute
	{
		GLTF_ATTRIBUTE_POSITION		= 0,
		GLTF_ATTRIBUTE_NORMAL		= 1,
		GLTF_ATTRIBUTE_TANGENT		= 2,
		GLTF_ATTRIBUTE_TEXCOORD_0	= 3,
		GLTF_ATTRIBUTE_TEXCOORD_1	= 4,
		GLTF_ATTRIBUTE_COLOR_0		= 5,
		GLTF_ATTRIBUTE_JOINTS_0		= 6,
		GLTF_ATTRIBUTE_WEIGHTS_0	= 7
	};

	enum GLTFAttributeBits
	{
		GLTF_ATTRIBUTE_POSITION_BIT		= 0x01,
		GLTF_ATTRIBUTE_NORMAL_BIT		= 0x02,
		GLTF_ATTRIBUTE_TANGENT_BIT		= 0x04,
		GLTF_ATTRIBUTE_TEXCOORD_0_BIT	= 0x08,
		GLTF_ATTRIBUTE_TEXCOORD_1_BIT	= 0x10,
		GLTF_ATTRIBUTE_COLOR_0_BIT		= 0x20,
		GLTF_ATTRIBUTE_JOINTS_0_BIT		= 0x40,
		GLTF_ATTRIBUTE_WEIGHTS_0_BIT	= 0x80
	};

	struct GLTFExtension
	{
		// TODO
	};

	struct GLTFExtra
	{
		// TODO
	};

	struct GLTFAsset
	{
		String					version;
		String					minVersion;
		String					generator;

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};

	struct GLTFNode
	{
		String					name;
		Int32					camera;
		Int32					mesh;
		Int32					skin;
		Array<Float32>			weights;
		Vector3					translation;
		Vector3					rotation;
		Vector3					scale;
		Array<UInt32>			children;

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};

	struct GLTFScene
	{
		String			name;
		Array<UInt32>	rootNodes;
	};

	struct GLTFScenes
	{
		Array<GLTFScene> scenes;
	};

	struct GLTFCamera
	{
		// TODO
	};

	struct GLTFAccessor
	{
		String			name;
		UInt32			bufferView;
		UInt32			byteOffset;
		GLTFComponent	componentType;
		Bool8			normalized;
		UInt32			count;
		GLTFType		type;
		Array<Float32>	min;
		Array<Float32>	max;

		// Sparse not supported

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};

	struct GLTFBuffer
	{
		String					name;
		String					uri;
		UInt32					byteLength;

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};
	
	struct GLTFBufferView
	{
		String					name;
		UInt32					buffer;
		UInt32					byteOffset;
		UInt32					byteLength;
		UInt32					byteStride;
		GLTFTarget				target;

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};

	struct GLTFPrimitive
	{
		Flags8					attributeFlags;
		UInt16					attributes[8];
		UInt32					indices;
		UInt32					material;	
		GLTFPrimitiveMode		mode;
		Flags8					morphTargetFlags;
		UInt16					morphTargets[8];

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};
	
	struct GLTFMesh
	{
		String					name;
		Array<GLTFPrimitive>	primitives;
		Array<Float32>			weights;

		#if GLTF_USE_EXTENTIONS
		Array<GLTFExtension>	extensions;
		Array<GLTFExtra>		extras;
		#endif
	};

	struct GLTFModel : public Resource<GLTFModel>
	{
		GLTFAsset				asset;
		Array<GLTFNode>			nodes;
		GLTFScenes				scenes;
		UInt32					scene;
		GLTFCamera				camera;
		Array<GLTFAccessor>		accessors;
		Array<GLTFBuffer>		buffers;
		Array<GLTFBufferView>	bufferViews;
		Array<GLTFMesh>			meshes;
	};
}