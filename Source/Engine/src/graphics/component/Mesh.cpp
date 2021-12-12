#include "Mesh.h"

#include "../../resource/loaders/OBJLoader.h"
#include "../../log/Log.h"

#include <iostream>
#include <fstream>

#include "../../Engine.h"

namespace Quartz
{
	MeshComponent::MeshComponent(const String& filepath)
		: filepath(filepath)
	{
		ResourceManager* pResourceManager = Engine::GetInstance()->GetResourceManager();
		pModel = pResourceManager->CheckoutResource<Model>(filepath);
	}
}

