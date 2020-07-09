#pragma once

#include "Common.h"
#include "util\String.h"
#include "util\Map.h"
#include "util\Buffer.h"
#include "math\Math.h"

namespace Quartz
{
	enum UniformType
	{
		UNIFORM_TYPE_UINT,
		UNIFORM_TYPE_INT,
		UNIFORM_TYPE_FLOAT,
		UNIFORM_TYPE_FLOAT2,
		UNIFORM_TYPE_FLOAT3,
		UNIFORM_TYPE_FLOAT4,
		UNIFORM_TYPE_MATRIX4
	};

	class QUARTZ_API UniformBlockData
	{
	private:
		struct UniformInfo
		{
			UInt32 index;
			String name;
			UniformType type;
			UInt32 sizeBytes;
		};

		Map<String, UniformInfo> mUniformTable;
		FloatBuffer mUniformBuffer;

	public:
		void AddFloat(const String& name, Float32 defaultValue = 0.0f);
		void AddFloat2(const String& name, Vector2 defaultValue = {});
		void AddFloat3(const String& name, Vector3 defaultValue = {});
		void AddFloat4(const String& name, Vector4 defaultValue = {});
		void AddMatrix4(const String& name, Matrix4 defaultValue = {});

		void SetFloat(const String& name, Float32 value);
		void SetFloat2(const String& name, Vector2 value);
		void SetFloat3(const String& name, Vector3 value);
		void SetFloat4(const String& name, Vector4 value);
		void SetMatrix4(const String& name, Matrix4 value);

		FloatBuffer& GetBuffer() { return mUniformBuffer; }
	};
}