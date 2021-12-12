#pragma once

#include "../../entity/Component.h"
#include "../object/Model.h"

namespace Quartz
{
	struct MeshComponent : public Component<MeshComponent>
	{
		String	filepath;
		Model*	pModel;

		MeshComponent(const String& filepath);
	};
}