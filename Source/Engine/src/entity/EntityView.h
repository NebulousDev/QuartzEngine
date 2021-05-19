#pragma once

#include "Common.h"
#include "Entity.h"
#include "util/Tuple.h"
#include "util/Storage.h"
#include "util/Utility.h"

namespace Quartz
{
	template<typename... Component>
	class EntityView
	{
	public:

		template<typename ComponentType>
		using ComponentStorage = Storage<ComponentType, Entity, Entity::HandleIntType>;
		using EntitySet = SparseSet<Entity, Entity::HandleIntType>;

		class Iterator
		{
		private:
			EntitySet::Iterator itr;
			EntityView<Component...>* pView;
		public:
			Iterator()
				: itr(EntitySet::Iterator(nullptr)) { }

			Iterator(EntitySet::Iterator itr, EntityView<Component...>* pView)
				: itr(itr), pView(pView) { }

			Iterator(const Iterator& it)
				: itr(it.itr), pView(it.pView) { }

			Iterator& operator++()
			{
				if constexpr (sizeof...(Component) == 1)
				{
					++itr;
					return *this;
				}
				else
				{
					while (true)
					{
						++itr;

						if (*this == pView->end() || 
							(pView->mStorages.Get<ComponentStorage<Component>*>()->Contains(itr->index) && ...))
						{
							return *this;
						}
					}

					return *this;
				}
			}

			Iterator operator++(int)
			{
				Iterator temp(*this);
				(*this)++;
				return *temp;
			}

			Bool8 operator==(const Iterator& it) const
			{
				return itr == it.itr;
			}

			Bool8 operator!=(const Iterator& it) const
			{
				return itr != it.itr;
			}

			Bool8 operator>=(const Iterator& it) const
			{
				return itr >= it.itr;
			}

			Bool8 operator<=(const Iterator& it) const
			{
				return itr <= it.itr;
			}

			Bool8 operator>(const Iterator& it) const
			{
				return itr > it.itr;
			}

			Bool8 operator<(const Iterator& it) const
			{
				return itr < it.itr;
			}

			Entity& operator*()
			{
				return *itr;
			}

			Entity* operator->()
			{
				return itr;
			}
		};

		friend class Iterator;

	private:
		Tuple<ComponentStorage<Component>*...>	mStorages;
		EntitySet*								mPrimarySet;
		
	private:
		SparseSet<Entity>* FindSmallest()
		{
			return FoldCompare
			(
				[](const EntitySet* set1, const EntitySet* set2)
				{
					return set1->Size() < set2->Size();
				},

				static_cast<EntitySet*>(mStorages.Get<ComponentStorage<Component>*>())...
			);
		}

	public:
		EntityView()
			: mStorages(), mPrimarySet(nullptr) { }

		EntityView(ComponentStorage<Component>*... sets)
			: mStorages(static_cast<ComponentStorage<Component>*>(sets)...),
			mPrimarySet(FindSmallest()) { }

		Iterator begin()
		{
			return mPrimarySet != nullptr ? Iterator(mPrimarySet->begin(), this) : Iterator();
		}

		Iterator end()
		{
			return  mPrimarySet != nullptr ? Iterator(mPrimarySet->end(), this) : Iterator();
		}
	};
}