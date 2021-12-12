#include "VulkanShader.h"

namespace Quartz
{
	VulkanShader::VulkanShader(const String& name, ShaderType type, VkShaderModule vkShader,
		VkShaderStageFlagBits vkStage, const String& entryPoint,
		const Array<SpirvUniform>& uniforms, const Array<SpirvAttribute>& attributes)
		: mvkShader(vkShader),
		mvkStage(vkStage),
		mEntryPoint(entryPoint),
		mUniforms(uniforms),
		mAttributes(attributes),
		GFXShader(name, type)
	{
		// Nothing
	}
}

