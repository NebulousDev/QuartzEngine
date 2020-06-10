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

		ValueType& Add(ValueType value)
		{
			return mTable.Insert(Hash(value), Move(value));
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

		typename Table<ValueType>::Iterator begin() noexcept
		{
			return mTable.begin();
		}

		typename Table<ValueType>::Iterator end() noexcept
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
			return mTable.mSize;
		}

		UInt32 Capacity() const
		{
			return mTable.mCapacity;
		}

		UInt32 Threshold() const
		{
			return mTable.mThreshold;
		}
	};
}