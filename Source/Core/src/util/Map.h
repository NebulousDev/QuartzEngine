#pragma once

#include "../Common.h"
#include "Table.h"

namespace Quartz
{
	template<typename _Key, typename _Value>
	struct MapPair
	{
		using MapPairType	= MapPair<_Key, _Value>;
		using KeyType		= _Key;
		using ValueType		= _Value;

		KeyType		key;
		ValueType	value;

		MapPair()
			: key(), value() {}

		MapPair(const KeyType& key)
			: key(key), value() { }

		MapPair(const KeyType& key, const ValueType& value)
			: key(key), value(value) { }

		MapPair(const MapPairType& pair)
			: key(pair.key), value(pair.value) {}

		MapPair(MapPairType&& pair) noexcept
		{
			Swap(*this, pair);
		}

		friend void Swap(MapPairType& pair1, MapPairType& pair2)
		{
			using Quartz::Swap;
			Swap(pair1.key, pair2.key);
			Swap(pair1.value, pair2.value);
		}

		MapPairType& operator=(MapPairType pair)
		{
			Swap(*this, pair);
			return *this;
		}

		Bool8 operator==(const MapPairType& pair)
		{
			return key == pair.key;
		}

		Bool8 operator!=(const MapPairType& pair)
		{
			return key != pair.key;
		}
	};

	template<typename _Key, typename _Value>
	class Map
	{
	public:
		using MapType	= Map<_Key, _Value>;
		using KeyType	= _Key;
		using ValueType = _Value;
		using PairType	= MapPair<KeyType, ValueType>;
		using TableType = Table<PairType>;

	private:
		TableType mTable;

	public:
		Map()
			: mTable() {}

		Map(UInt32 size)
			: mTable(size) {}

		Map(const MapType& map)
			: mTable(map.mTable) {}

		Map(MapType&& map) noexcept
		{
			Swap(*this, map);
		}

		friend void Swap(MapType& map1, MapType& map2)
		{
			using Quartz::Swap;
			Swap(map1.mTable, map2.mTable);
		}

		ValueType& Put(const KeyType& key, const ValueType& value)
		{
			UInt32 hash = Hash(key);
			return mTable.Insert(hash, PairType(key, value)).value;
		}

		MapType& operator=(MapType map)
		{
			Swap(*this, map);
			return *this;
		}

		// TODO: WARNING!!! Use Put()
		// [] will place default ValueType() which can sometimes be
		// equal to an existing entry.
		ValueType& operator[](const KeyType& key)
		{
			ValueType* pValue = Get(key);

			if (pValue != nullptr)
			{
				return *pValue;
			}

			return Put(key, ValueType());
		}

		typename TableType::Iterator begin()
		{
			return mTable.begin();
		}

		typename TableType::Iterator end()
		{
			return mTable.end();
		}

		typename TableType::Iterator begin() const
		{
			return const_cast<MapType*>(this)->mTable.begin();
		}

		typename TableType::Iterator end() const
		{
			return const_cast<MapType*>(this)->mTable.end();
		}

		PairType* Find(const KeyType& key)
		{
			return mTable.Find(Hash(key), PairType(key));
		}

		ValueType* Get(const KeyType& key)
		{
			PairType* pPair = mTable.Find(Hash(key), PairType(key));
			return pPair == nullptr ? nullptr : &pPair->value;
		}

		Bool8 Reserve(UInt32 size)
		{
			return mTable.Reserve(size);
		}

		void Shrink()
		{
			mTable.Shrink();
		}

		void Clear()
		{
			mTable.Clear();
		}

		Bool8 Contains(const KeyType& key)
		{
			return mTable.Contains(Hash(key), PairType(key));
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