#include "Pipeline.h"

namespace Quartz
{
	Pipeline::Pipeline(PipelineType type)
		: mType(type)
	{
		// Nothing
	}

	GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineInfo& info)
		: mPipelineInfo(info), Pipeline(PIPELINE_TYPE_GRAPHICS)
	{
		// Nothing
	}
}

