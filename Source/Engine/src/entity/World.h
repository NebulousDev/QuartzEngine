#pragma once

#include "EntityDatabase.h"
#include "EntityGraph.h"

#include "../log/Log.h"

namespace Quartz
{
	class QUARTZ_API EntityWorld
	{
		friend class Engine;

	private:
		EntityDatabase*	mpDatabase;
		EntityGraph*	mpGraph;

	private:
		EntityWorld();
		void Initialize(EntityDatabase* pDatabase, EntityGraph* pGraph);

	public:
		EntityWorld(EntityDatabase* pDatabase, EntityGraph* pGraph);

		Bool8 IsValid(Entity entity);
		Bool8 SetParent(Entity entity, Entity parent);
		Bool8 RemoveParent(Entity entity);

		void Refresh(Entity entity);

		template<typename... Component>
		Entity CreateEntity(Component&&... component)
		{
			Entity entity = mpDatabase->CreateEntity(std::forward<Component>(component)...);
			mpGraph->ParentEntityToRoot(entity);
			return entity;
		}

		template<typename... Component>
		Entity CreateEntityParented(Entity parent, Component&&... component)
		{
			Entity entity = mpDatabase->CreateEntity(std::forward<Component>(component)...);

			if (!mpGraph->ParentEntity(entity, parent))
			{
				mpGraph->ParentEntityToRoot(entity);
				Log::Warning("Attempted to parent entity [%X] to invalid parent [%X]!", entity, parent);
			}

			return entity;
		}

		template<typename... Component>
		void AddComponent(Entity entity, Component&&... component)
		{
			if (!IsValid(entity))
			{
				Log::Warning("Attempted to add component to invalid entity [%X]", entity);
				return;
			}
			
			mpDatabase->AddComponent(entity, std::forward<Component>(component)...);
		}

		template<typename... Component>
		void RemoveComponent(Entity entity)
		{
			mpDatabase->RemoveComponent<Component...>(entity);
		}

		template<typename... Component>
		Bool8 HasComponent(Entity entity)
		{
			return mpDatabase->HasComponent<Component...>(entity);
		}

		/* Assumes entity has component. Undefiened otherwise.*/
		template<typename Component>
		Component& GetComponent(Entity entity)
		{
			return mpDatabase->GetComponent<Component>(entity);
		}

		template<typename... Component>
		EntityView<Component...> CreateView()
		{
			return mpDatabase->CreateView<Component...>();
		}

		FORCE_INLINE EntityDatabase&	GetDatabase() { return *mpDatabase; }
		FORCE_INLINE EntityGraph&		GetGraph() { return *mpGraph;}
	};
}