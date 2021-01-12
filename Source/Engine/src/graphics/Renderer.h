#pragma once

#include "VGFXContext.h"
#include "../object/Model.h"

namespace Quartz
{
	struct RenderScene
	{
		Array<Model*> models;
	};

	class QUARTZ_API Renderer
	{
	private:
		RenderScene* mpScene;

	public:
		
		// @Todo: This is temporary
		void SetScene(RenderScene& scene);

		void Render();
	};
}