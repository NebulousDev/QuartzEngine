#pragma once

#include "SparseSet.h"

namespace Quartz
{
	template<typename _ValueType, typename _HandleType = UInt32, typename _IntType = UInt32, USize blockSize = 64>
	class Storage : public SparseSet<_HandleType, _IntType, blockSize>
	{
	public:
		using ValueType		= _ValueType;
		using HandleType	= _HandleType;
		using IntType		= _IntType;
		using BaseSet		= SparseSet<_HandleType, _IntType, blockSize>;
		using Iterator		= typename Array<ValueType>::Iterator;
	
	private:
		Array<ValueType> mStorage;

	public:
		Storage()
			: BaseSet(), mStorage() {}

		template<typename Value>
		ValueType& Insert(HandleType handle, Value&& value)
		{
			IntType index = BaseSet::Insert(handle);

			if (index < mStorage.Size())
			{
				return mStorage[index] = std::forward<Value>(value);
			}

			return *mStorage.PushBack(std::forward<Value>(value));
		}

		void Remove(HandleType handle)
		{
			assert(BaseSet::Contains(handle) && "Storage does not contain handle!");

			IntType index = BaseSet::Index(handle);
			//Swap(mStorage[index], mStorage[mStorage.Size() - 1]);
			mStorage[index] = mStorage[mStorage.Size() - 1];
			mStorage.PopBack();

			BaseSet::Remove(handle);
		}

		Bool8 Contains(HandleType handle) const
		{
			return BaseSet::Contains(handle);
		}

		ValueType& Get(HandleType handle)
		{
			assert(BaseSet::Contains(handle) && "Storage does not contain handle!");
			return mStorage[BaseSet::Index(handle)];
		}

		Iterator begin()
		{
			return mStorage.begin();
		}

		Iterator end()
		{
			return mStorage.end();
		}

		USize Size() const
		{
			return BaseSet::Size();
		}
	};
}