#pragma once

#include "util\Array.h"
#include "util\Map.h"
#include "util\TypeId.h"
#include "util\Buffer.h"
#include "util\Storage.h"

#include "Entity.h"
#include "EntityView.h"

namespace Quartz
{
	struct EntityEvent
	{

	};

	class EntityWorld
	{
	public:

		template<typename ComponentType>
		using ComponentStorage = Storage<ComponentType, Entity, Entity::HandleIntType>;
		using EntitySet = SparseSet<Entity, Entity::HandleIntType>;

	private:
		struct ComponentTypeCounter
		{
			static USize Next()
			{
				static USize index = 0;
				return index++;
			}
		};

		template<typename ComponentType>
		struct ComponentTypeIndex
		{
			static USize Value()
			{
				static USize index = ComponentTypeCounter::Next();
				return index;
			}
		};

		struct SystemTypeCounter
		{
			static USize Next()
			{
				static USize index = 0;
				return index++;
			}
		};

		template<typename ComponentType>
		struct SystemTypeIndex
		{
			static USize Value()
			{
				static USize index = SystemTypeCounter::Next();
				return index;
			}
		};

	private:
		Array<EntitySet*>	mStorageSets;
		Array<Entity>		mEntites;

	private:
		template<typename Component>
		Bool8 HasComponentImpl(Entity entity)
		{
			using ComponentType = std::decay_t<Component>;
			USize typeIndex = ComponentTypeIndex<ComponentType>::Value();

			if (typeIndex >= mStorageSets.Size())
			{
				return false;
			}

			ComponentStorage<Component>& storage = 
				*static_cast<ComponentStorage<Component>*>(mStorageSets[typeIndex]);

			return storage.Contains(entity.index);
		}

		template<typename Component>
		void AddComponentImpl(Entity entity, Component&& component)
		{
			using ComponentType = std::decay_t<Component>;
			USize typeIndex = ComponentTypeIndex<ComponentType>::Value();

			if (typeIndex >= mStorageSets.Size())
			{
				mStorageSets.Resize(typeIndex + 1, nullptr);
				mStorageSets[typeIndex] = new ComponentStorage<ComponentType>();
			}

			static_cast<ComponentStorage<ComponentType>*>
				(mStorageSets[typeIndex])->Insert(entity.index, std::forward<Component>(component));
		}

		template<typename Component>
		void RemoveComponentImpl(Entity entity)
		{
			using ComponentType = std::decay_t<Component>;
			USize typeIndex = ComponentTypeIndex<ComponentType>::Value();
			static_cast<ComponentStorage<ComponentType>*>(mStorageSets[typeIndex])->Remove(entity.index);
		}

	public:
		template<typename... Component>
		Entity CreateEntity(Component&&... component)
		{
			Entity entity = *mEntites.PushBack(Entity(mEntites.Size() + 1, 0));
			AddComponent(entity, std::forward<Component>(component)...);
			return entity;
		}

		template<typename... Component>
		void AddComponent(Entity entity, Component&&... component)
		{
			(AddComponentImpl<Component>(entity, std::forward<Component>(component)), ...);
		}

		template<typename... Component>
		void RemoveComponent(Entity entity)
		{
			(RemoveComponentImpl<Component>(entity), ...);
		}

		template<typename... Component>
		Bool8 HasComponent(Entity entity)
		{
			return (HasComponentImpl<Component>(entity) && ...);
		}

		/* Assumes entity has component. Undefiened otherwise.*/
		template<typename Component>
		Component& GetComponent(Entity entity)
		{
			using ComponentType = std::decay_t<Component>;
			USize typeIndex = ComponentTypeIndex<ComponentType>::Value();
			return static_cast<ComponentStorage<ComponentType>*>(mStorageSets[typeIndex])->Get(entity.index);
		}

		template<typename Component>
		Bool8 ComponentExists()
		{
			return ComponentTypeIndex<Component>::Value() < mStorageSets.Size();
		}

		template<typename... Component>
		EntityView<Component...> CreateView()
		{
			if ((!ComponentExists<Component>() || ...))
			{
				// One or more components does not exist in this world
				return EntityView<Component...>();
			}

			return EntityView<Component...>(
				static_cast<ComponentStorage<Component>*>(mStorageSets[ComponentTypeIndex<Component>::Value()])...);
		}
	};
}