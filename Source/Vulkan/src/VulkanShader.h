#pragma once

#include "graphics/Shader.h"
#include "SPIRVUtil.h"

namespace Quartz
{
	class QUARTZ_API VulkanShader : public Shader
	{
	private:
		VkShaderModule			mvkShader;
		VkShaderStageFlagBits	mvkStage;
		String					mEntryPoint;
		Array<SpirvUniform>		mUniforms;
		Array<SpirvAttribute>	mAttributes;

	public:
		VulkanShader(const String& name, ShaderType type, VkShaderModule vkShader,
			VkShaderStageFlagBits vkStage, const String& entryPoint,
			const Array<SpirvUniform>& uniforms, const Array<SpirvAttribute>& attributes);

		FORCE_INLINE VkShaderModule					GetVkShader() { return mvkShader; }
		FORCE_INLINE VkShaderStageFlagBits			GetVkShaderStage() { return mvkStage; }
		FORCE_INLINE String							GetEntryPoint() { return mEntryPoint; }
		FORCE_INLINE const Array<SpirvUniform>&		GetUniforms() { return mUniforms; }
		FORCE_INLINE const Array<SpirvAttribute>&	GetAttributes() { return mAttributes; }
	};
}