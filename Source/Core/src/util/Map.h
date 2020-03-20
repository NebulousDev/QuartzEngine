#pragma once
#include "Array.h"
#include "Hash.h"

#include <initializer_list>

namespace Quartz
{
	template<typename Key, typename Value>
	struct HashEntry
	{
		Key key;
		Value value;
		UInt32 hash;

		HashEntry();
		HashEntry(const Key& key, const Value& value);
		HashEntry(const Key& key, const Value& value, const UInt32 hash);
	};

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&) = Hash<Key>>
	class Map
	{
	public:
		constexpr static Float32 LOAD_FACTOR = 0.85f;

		using MapType = Map<Key, Value, HashFunc>;
		using EntryType = HashEntry<Key, Value>;

	public:
		class Iterator
		{
		private:
			EntryType* pItr;
			EntryType* pEnd;

		public:
			Iterator(EntryType* pItr, EntryType* pEnd);
			Iterator(const Iterator& it);
			Iterator(Iterator&& it) noexcept;

			Iterator& operator++();
			Iterator operator++(int);

			Bool8 operator==(const Iterator& it) const;
			Bool8 operator!=(const Iterator& it) const;
			Bool8 operator>=(const Iterator& it) const;
			Bool8 operator<=(const Iterator& it) const;
			Bool8 operator>(const Iterator& it) const;
			Bool8 operator<(const Iterator& it) const;

			Iterator& operator=(const Iterator& it);

			EntryType& operator*();
			EntryType* operator->();

			Bool8 IsValid() const;
		};

	private:
		Array<HashEntry<Key, Value>> mTable;
		UInt32 mSize;
		UInt32 mCapacity;
		UInt32 mThreshold;
		UInt32 mMask;

		FORCE_INLINE Bool8 IsDeleted(const UInt32 hash) const;
		FORCE_INLINE UInt32 CalcIndex(const UInt32 hash) const;
		FORCE_INLINE UInt32 CalcHash(const Key& key) const;
		FORCE_INLINE UInt32 CalcDist(const UInt32 hash, const UInt32 index) const;

		void GrowImpl();

		HashEntry<Key, Value>& PutImpl(Key&& key, Value&& value);
		
		void InsertImpl(const UInt32 index, const UInt32 hash, const Key& key, const Value& value);
		void InsertImpl(Array<HashEntry<Key, Value>>& table, const UInt32 index, 
			const UInt32 hash, const Key& key, const Value& value);

	public:
		Map();
		Map(const Map& map);

		Map(const std::initializer_list<HashEntry<Key, Value>>& list);

		HashEntry<Key, Value>& Put(Key key, Value value);

		Iterator Find(const Key& key);

		Value* Get(const Key& key);
		const Value* Get(const Key& key) const;

		void Erase(const Key& key);
		void Erase(Iterator& it);

		Bool8 Contains(const Key& key);

		Value& operator[](const Key& key);

		Iterator begin();
		Iterator end();

		FORCE_INLINE UInt32 GetSize() const { return mSize; }
		FORCE_INLINE UInt32 GetCapacity() const { return mCapacity; }
		FORCE_INLINE UInt32 GetLoadFactor() const { return LOAD_FACTOR; }
	};

	template<typename Key, typename Value>
	HashEntry<Key, Value>::HashEntry()
		: key{}, value{}, hash(0)
	{
		// Nothing
	}

	template<typename Key, typename Value>
	HashEntry<Key, Value>::HashEntry(const Key& key, const Value& value)
		: key(key), value(value), hash(0)
	{
		// Nothing
	}

	template<typename Key, typename Value>
	HashEntry<Key, Value>::HashEntry(const Key& key, const Value& value, const UInt32 hash)
		: key(key), value(value), hash(hash)
	{
		// Nothing
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Map<Key, Value, HashFunc>::Iterator::Iterator(HashEntry<Key, Value>* pItr, HashEntry<Key, Value>* pEnd)
		: pItr(pItr), pEnd(pEnd)
	{
		// Nothing
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Map<Key, Value, HashFunc>::Iterator::Iterator(const Iterator& it)
		: pItr(it.pItr), pEnd(it.pEnd)
	{
		// Nothing
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Map<Key, Value, HashFunc>::Iterator::Iterator(Iterator&& it) noexcept
		: pItr(it.pItr), pEnd(it.pEnd)
	{
		// Nothing
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	typename Map<Key, Value, HashFunc>::Iterator& Map<Key, Value, HashFunc>::Iterator::operator++()
	{
		while ((!(++pItr)->hash || (pItr->hash >> 31)) && pItr < pEnd);
		return *this;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	typename Map<Key, Value, HashFunc>::Iterator Map<Key, Value, HashFunc>::Iterator::operator++(int)
	{
		Map<Key, Value, HashFunc>::Iterator temp(*this);
		while ((!(++pItr)->hash || (pItr->hash >> 31)) && pItr < pEnd);
		return temp;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::operator==(const Iterator& it) const
	{
		return pItr == it.pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::operator!=(const Iterator& it) const
	{
		return pItr != it.pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::operator>=(const Iterator& it) const
	{
		return pItr >= it.pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::operator<=(const Iterator& it) const
	{
		return pItr <= it.pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::operator>(const Iterator& it) const
	{
		return pItr > it.pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::operator<(const Iterator& it) const
	{
		return pItr < it.pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	typename Map<Key, Value, HashFunc>::Iterator& Map<Key, Value, HashFunc>::Iterator::operator=(const Iterator& it)
	{
		pItr = it.pItr;
		return *this;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	HashEntry<Key, Value>& Map<Key, Value, HashFunc>::Iterator::operator*()
	{
		return *pItr;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	HashEntry<Key, Value>* Map<Key, Value, HashFunc>::Iterator::operator->()
	{
		return pItr;
	}

	// TODO / NOTE: THIS IS A WORKAROUND TO THE ITERATOR NOT WORKING!

	template<typename Key, typename Value, UInt32(*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::Iterator::IsValid() const
	{
		return pItr->hash && ((pItr->hash >> 31) != 0) && pItr != pEnd;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Bool8 Map<Key, Value, HashFunc>::IsDeleted(const UInt32 hash) const
	{
		return (hash >> 31) != 0;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	UInt32 Map<Key, Value, HashFunc>::CalcIndex(const UInt32 hash) const
	{
		return hash & mMask;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	UInt32 Map<Key, Value, HashFunc>::CalcHash(const Key& key) const
	{
		UInt32 hash = HashFunc(key);
		hash &= 0x7FFFFFFF;
		hash |= hash == 0;
		return hash;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	UInt32 Map<Key, Value, HashFunc>::CalcDist(const UInt32 hash, const UInt32 index) const
	{
		return (index + mCapacity - (hash & mMask) & mMask);
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	void Map<Key, Value, HashFunc>::GrowImpl()
	{
		Array<EntryType> newTable;

		const UInt32 oldCapacity = mCapacity;

		mCapacity *= 2;
		mThreshold = (UInt32)(mCapacity * LOAD_FACTOR);
		mMask = mCapacity - 1;

		newTable.Resize(mCapacity, HashEntry<Key, Value>());

		for (UInt32 i = 0; i < oldCapacity; ++i)
		{
			EntryType& pair = mTable[i];
			UInt32 hash = pair.hash;

			if (hash != 0 && !IsDeleted(hash))
				InsertImpl(newTable, CalcIndex(hash), hash, pair.key, pair.value);
		}

		mTable = newTable;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	HashEntry<Key, Value>& Map<Key, Value, HashFunc>::PutImpl(Key&& key, Value&& value)
	{
		UInt32 hash = CalcHash(key);
		UInt32 position = hash & mMask;
		UInt32 original = position;
		Bool8 keepPos = true;
		UInt32 dist = 0;

		while (true)
		{
			if (mTable[position].hash == 0)
			{
				InsertImpl(position, hash, key, value);
				return mTable[original];
			}

			UInt32 currentDist = CalcDist(mTable[position].hash, position);

			if (currentDist < dist)
			{
				if (IsDeleted(mTable[position].hash))
				{
					InsertImpl(position, hash, key, value);
					return mTable[original];
				}

				std::swap(hash, mTable[position].hash);
				std::swap(key, mTable[position].key);
				std::swap(value, mTable[position].value);
				dist = currentDist;

				if (keepPos)
				{
					original = position;
					keepPos = false;
				}
			}

			position = (position + 1) & mMask;
			++dist;
		}
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	void Map<Key, Value, HashFunc>::InsertImpl(const UInt32 index, const UInt32 hash, const Key& key, const Value& value)
	{
		new (&mTable[index]) HashEntry<Key, Value>(key, value, hash);
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	void Map<Key, Value, HashFunc>::InsertImpl(Array<HashEntry<Key, Value>>& table,
		const UInt32 index, const UInt32 hash, const Key& key, const Value& value)
	{
		new (&table[index]) HashEntry<Key, Value>(key, value, hash);
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Map<Key, Value, HashFunc>::Map()
		: mSize(0), mCapacity(16), mThreshold((UInt32)(16 * LOAD_FACTOR)), mMask(mCapacity - 1)
	{
		mTable.Resize(mCapacity, HashEntry<Key, Value>());
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Map<Key, Value, HashFunc>::Map(const Map& map)
		: mSize(map.mSize), mCapacity(map.mCapacity), mThreshold(map.mThreshold), mMask(map.mMask),
		mTable(map.mTable)
	{
		// Nothing
	}

	template<typename Key, typename Value, UInt32(*HashFunc)(const Key &)>
	Map<Key, Value, HashFunc>::Map(const std::initializer_list<HashEntry<Key, Value>>& list)
		: Map()
	{
		for (const HashEntry<Key, Value>* pEntry = list.begin(); pEntry != list.end(); ++pEntry)
			(*this)[pEntry->key] = pEntry->value;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	HashEntry<Key, Value>& Map<Key, Value, HashFunc>::Put(Key key, Value value)
	{
		if (++mSize >= mThreshold)
			GrowImpl();

		return PutImpl(std::move(key), std::move(value));
 	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	typename Map<Key, Value, HashFunc>::Iterator Map<Key, Value, HashFunc>::Find(const Key& key)
	{
		const UInt32 hash = CalcHash(key);
		UInt32 index = CalcIndex(hash);
		UInt32 dist = 0;

		while (true)
		{
			if (mTable[index].hash == 0 || dist > CalcDist(mTable[index].hash, index))
				return Map<Key, Value, HashFunc>::Iterator(&mTable[0] + mCapacity, &mTable[0] + mCapacity);
			else if (mTable[index].hash == hash && mTable[index].key == key)
				return Map<Key, Value, HashFunc>::Iterator(&mTable[index], &mTable[0] + mCapacity);

			index = (index + 1) & mMask;
			++dist;
		}
	}

	template<typename Key, typename Value, UInt32(*HashFunc)(const Key &)>
	Value* Map<Key, Value, HashFunc>::Get(const Key& key)
	{
		Map<Key, Value, HashFunc>::Iterator it = Find(key);
		return it == end() ? nullptr : &(it->value);
	}

	template<typename Key, typename Value, UInt32(*HashFunc)(const Key &)>
	const Value* Map<Key, Value, HashFunc>::Get(const Key& key) const
	{
		//TODO: This is awful, please implement a const iterator for this to work...
		return const_cast<Map<Key, Value, HashFunc>*>(this)->Get(key);
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	void Map<Key, Value, HashFunc>::Erase(const Key& key)
	{
		Map<Key, Value, HashFunc>::Iterator it = Find(key);

		assert(it != end() && "Key does not exist");

		it->hash |= (0x1 << 31);
		it->value.~Value();
		--mSize;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	void Map<Key, Value, HashFunc>::Erase(Iterator& it)
	{
		assert(it != end() && "Key does not exist");

		it->hash |= (0x1 << 31);
		it->value.~Value();
		--mSize;
	}

	template<typename Key, typename Value, UInt32(*HashFunc)(const Key &)>
	Bool8 Map<Key, Value, HashFunc>::Contains(const Key& key)
	{
		return Find(key) != end();
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	Value& Map<Key, Value, HashFunc>::operator[](const Key& key)
	{
		Map<Key, Value, HashFunc>::Iterator it = Find(key);

		if (it != end())
			return it->value;

		return Put(key, Value()).value;
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	typename Map<Key, Value, HashFunc>::Iterator Map<Key, Value, HashFunc>::begin()
	{
		return ++Map<Key, Value, HashFunc>::Iterator(&mTable[0], &mTable[0] + mCapacity);
	}

	template<typename Key, typename Value, UInt32 (*HashFunc)(const Key&)>
	typename Map<Key, Value, HashFunc>::Iterator Map<Key, Value, HashFunc>::end()
	{
		return Map<Key, Value, HashFunc>::Iterator(&mTable[0] + mCapacity, &mTable[0] + mCapacity);
	}

}