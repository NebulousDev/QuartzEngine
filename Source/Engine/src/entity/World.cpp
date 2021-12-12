#include "World.h"

namespace Quartz
{
	EntityWorld::EntityWorld()
		: mpDatabase(nullptr),
		mpGraph(nullptr)
	{
		// Nothing
	}

	void EntityWorld::Initialize(EntityDatabase* pDatabase, EntityGraph* pGraph)
	{
		mpDatabase = pDatabase;
		mpGraph = pGraph;
	}

	EntityWorld::EntityWorld(EntityDatabase* pDatabase, EntityGraph* pGraph)
		: mpDatabase(pDatabase), mpGraph(pGraph)
	{
		// Nothing
	}

	Bool8 EntityWorld::IsValid(Entity entity)
	{
		if (entity == NullEntity)
		{
			return false;
		}

		return mpDatabase->EntityExists(entity);
	}

	Bool8 EntityWorld::SetParent(Entity entity, Entity parent)
	{
		if (!IsValid(entity))
		{
			Log::Warning("Attempted to parent invalid entity [%X]", entity);
			return false;
		}

		if (!IsValid(parent))
		{
			Log::Warning("Attempted to parent entity [%X] to invalid parent [%X]!", entity, parent);
			return false;
		}

		mpGraph->ParentEntity(entity, parent);

		return true;
	}

	Bool8 EntityWorld::RemoveParent(Entity entity)
	{
		if (!IsValid(entity))
		{
			Log::Warning("Attempted to parent invalid entity [%X]", entity);
			return false;
		}

		mpGraph->ParentEntityToRoot(entity);

		return true;
	}

	void EntityWorld::Refresh(Entity entity)
	{
		mpGraph->Update(entity);
	}
}

