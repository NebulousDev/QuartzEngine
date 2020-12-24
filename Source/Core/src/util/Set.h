#pragma once

#include "../Common.h"
#include "Table.h"

namespace Quartz
{
	template<typename _Value>
	class Set
	{
	public:
		using SetType	= Set<_Value>;
		using ValueType = _Value;
		using Iterator  = typename Table<ValueType>::Iterator;

	private:
		Table<ValueType> mTable;

	public:
		Set()
			: mTable() {}

		Set(UInt32 size)
			: mTable(size) {}

		Set(const SetType& set)
			: mTable(set.mTable) {}

		Set(SetType&& set) noexcept
		{
			Swap(*this, set);
		}

		friend void Swap(SetType& set1, SetType& set2)
		{
			using Quartz::Swap;
			Swap(set1.mTable, set2.mTable);
		}

		template<typename Value>
		ValueType& Add(Value&& value)
		{
			return mTable.Insert(Hash<ValueType>(value), std::forward<Value>(value));
		}

		/*void Remove(const ValueType& value)
		{
			mTable.Remove(Hash(value), value);
		}*/

		SetType& operator=(SetType set)
		{
			Swap(*this, set);
			return *this;
		}

		void Clear()
		{
			mTable.Clear();
		}

		Iterator begin()
		{
			return mTable.begin();
		}

		Iterator end()
		{
			return mTable.end();
		}

		Bool8 Reserve(UInt32 size)
		{
			return mTable.Reserve(size);
		}

		void Shrink()
		{
			mTable.Shrink();
		}

		Bool8 Contains(const ValueType& value)
		{
			return mTable.Contains(Hash(value), value);
		}

		UInt32 Size() const
		{
			return mTable.Size();
		}

		UInt32 Capacity() const
		{
			return mTable.Capacity();
		}

		UInt32 Threshold() const
		{
			return mTable.Threshold();
		}
	};
}