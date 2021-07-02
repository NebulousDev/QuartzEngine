#pragma once

#include "graphics/Uniform.h"
#include "VulkanBuffer.h"

#include "util/Array.h"

namespace Quartz
{
	class QUARTZ_API VulkanUniform : public Uniform
	{
		typedef Byte* MappedData;

	private:
		VulkanDevice*			mpDevice;
		Array<MappedData>		mpMappedBuffers;
		Array<VulkanBuffer*>	mpStagingBuffers;
		Array<VulkanBuffer*>	mpUniformBuffers;
		UInt32					mAlignedSize;
		Bool8					mBuilt;

	public:
		VulkanUniform(VulkanDevice* pDevice, UniformType type, UInt32 elementSize, UInt32 elementCount, UniformFlags flags);

		void Set(void* pData) override;
		void SetElement(Context* pContext, UInt32 element, void* pData) override;

		void BuildBuffers(UInt32 bufferCount);

		FORCE_INLINE const Array<VulkanBuffer*>&	GetStagingBuffers() const { return mpStagingBuffers; }
		FORCE_INLINE const Array<VulkanBuffer*>&	GetUniformBuffers() const { return mpUniformBuffers; }
		FORCE_INLINE UInt32							GetAlignedElementSize() const { return mAlignedSize; }
		FORCE_INLINE Bool8							IsBuilt() const { return mBuilt; }
	};
}