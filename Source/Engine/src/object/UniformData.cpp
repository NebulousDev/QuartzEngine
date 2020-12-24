#include "UniformData.h"

#include <stdlib.h>

namespace Quartz
{
	void UniformBlockData::AddFloat(const String& name, Float32 defaultValue)
	{
		UniformInfo uniformInfo;
		uniformInfo.name = name;
		uniformInfo.type = UNIFORM_TYPE_FLOAT;
		uniformInfo.sizeBytes = sizeof(Float32);

		UInt32 index = mUniformBuffer.Push(defaultValue);
		uniformInfo.index = index;

		mUniformTable.Put(name, uniformInfo);
	}

	void UniformBlockData::AddFloat2(const String& name, Vector2 defaultValue)
	{
		UniformInfo uniformInfo;
		uniformInfo.name = name;
		uniformInfo.type = UNIFORM_TYPE_FLOAT2;
		uniformInfo.sizeBytes = sizeof(Vector2);

		UInt32 index = mUniformBuffer.Push(defaultValue);
		uniformInfo.index = index;

		mUniformTable.Put(name, uniformInfo);
	}

	void UniformBlockData::AddFloat3(const String& name, Vector3 defaultValue)
	{
		UniformInfo uniformInfo;
		uniformInfo.name = name;
		uniformInfo.type = UNIFORM_TYPE_FLOAT3;
		uniformInfo.sizeBytes = sizeof(Vector3);

		UInt32 index = mUniformBuffer.Push(defaultValue);
		uniformInfo.index = index;

		mUniformTable.Put(name, uniformInfo);
	}

	void UniformBlockData::AddFloat4(const String& name, Vector4 defaultValue)
	{
		UniformInfo uniformInfo;
		uniformInfo.name = name;
		uniformInfo.type = UNIFORM_TYPE_FLOAT3;
		uniformInfo.sizeBytes = sizeof(Vector4);

		UInt32 index = mUniformBuffer.Push(defaultValue);
		uniformInfo.index = index;

		mUniformTable.Put(name, uniformInfo);
	}

	void UniformBlockData::AddMatrix4(const String& name, Matrix4 defaultValue)
	{
		UniformInfo uniformInfo;
		uniformInfo.name = name;
		uniformInfo.type = UNIFORM_TYPE_MATRIX4;
		uniformInfo.sizeBytes = sizeof(Matrix4);

		UInt32 index = mUniformBuffer.Push(defaultValue);
		uniformInfo.index = index;

		mUniformTable.Put(name, uniformInfo);
	}

	void UniformBlockData::SetFloat(const String& name, Float32 value)
	{
		UniformInfo* pInfo = mUniformTable.Get(name);

		if (pInfo == nullptr || pInfo->type != UNIFORM_TYPE_FLOAT)
		{
			// Do something?
			return;
		}

		mUniformBuffer.Get<Float32>(pInfo->index) = value;
	}

	void UniformBlockData::SetFloat2(const String& name, Vector2 value)
	{
		UniformInfo* pInfo = mUniformTable.Get(name);

		if (pInfo == nullptr || pInfo->type != UNIFORM_TYPE_FLOAT2)
		{
			// Do something?
			return;
		}

		mUniformBuffer.Get<Vector2>(pInfo->index) = value;
	}

	void UniformBlockData::SetFloat3(const String& name, Vector3 value)
	{
		UniformInfo* pInfo = mUniformTable.Get(name);

		if (pInfo == nullptr || pInfo->type != UNIFORM_TYPE_FLOAT3)
		{
			// Do something?
			return;
		}

		mUniformBuffer.Get<Vector3>(pInfo->index) = value;
	}

	void UniformBlockData::SetFloat4(const String& name, Vector4 value)
	{
		UniformInfo* pInfo = mUniformTable.Get(name);

		if (pInfo == nullptr || pInfo->type != UNIFORM_TYPE_FLOAT3)
		{
			// Do something?
			return;
		}

		mUniformBuffer.Get<Vector4>(pInfo->index) = value;
	}

	void UniformBlockData::SetMatrix4(const String& name, Matrix4 value)
	{
		UniformInfo* pInfo = mUniformTable.Get(name);

		if (pInfo == nullptr || pInfo->type != UNIFORM_TYPE_MATRIX4)
		{
			// Do something?
			return;
		}

		mUniformBuffer.Get<Matrix4>(pInfo->index) = value;
	}
}

