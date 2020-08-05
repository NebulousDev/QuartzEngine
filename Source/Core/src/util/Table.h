#pragma once

#include "Array.h"
#include "Hash.h"
#include "Utils.h"

namespace Quartz
{
	template<typename _KeyValueType>
	class TableEntry
	{
	public:
		using TableEntryType	= TableEntry<_KeyValueType>;
		using KeyValueType		= _KeyValueType;

	public:
		KeyValueType	keyValue;
		UInt32			hash;

		union
		{
			struct
			{
			Bool8 isFirst : 1;
			Bool8 isLast : 1;
			UInt8 reserved : 5;
			Bool8 isDeleted : 1;
			};

			UInt8 meta;
		};

	public:
		TableEntry() :
			keyValue(), hash(0), meta(0) { }

		TableEntry(UInt32 hash, KeyValueType&& keyValue) :
			hash(hash), keyValue(Move(keyValue)), meta(0) {}

		TableEntry(UInt32 hash, const KeyValueType& keyValue) :
			hash(hash), keyValue(keyValue), meta(0) {}

		TableEntry(const TableEntry& entry) :
			hash(entry.hash), keyValue(entry.keyValue), meta(entry.meta) {}

		TableEntry(TableEntry&& entry) noexcept :
			TableEntry()
		{
			Swap(*this, entry);
		}

		TableEntryType& operator=(TableEntryType entry)
		{
			Swap(*this, entry);
			return *this;
		}

		friend void Swap(TableEntryType& entry1, TableEntryType& entry2)
		{
			using Quartz::Swap;
			Swap(entry1.hash, entry2.hash);
			Swap(entry1.keyValue, entry2.keyValue);
			// Dont swap meta
		}

		Bool8 IsEmpty()
		{
			return hash == 0;
		}

		Bool8 IsDeleted()
		{
			return isDeleted;
		}

		Bool8 IsFirst()
		{
			return isFirst;
		}

		Bool8 IsLast()
		{
			return isLast;
		}
	};

	template<typename _KeyValueType>
	class Table
	{
	public:
		using KeyValueType = _KeyValueType;
		using TableType = Table<KeyValueType>;
		using TableEntryType = TableEntry<KeyValueType>;

		constexpr static Float32	LOAD_FACTOR = 0.85f;
		constexpr static USize		INITAL_SIZE = 16;

		constexpr static UInt32		ERASED_BIT = (0x1 << 31);

		class Iterator
		{
		private:
			TableEntryType* pItr;

		public:
			Iterator()
				: pItr(nullptr) { }

			Iterator(TableEntryType* pItr)
				: pItr(pItr) { }

			Iterator(const Iterator& it)
				: pItr(it.pItr) { }

			Iterator& operator++()
			{
				do
				{
					if (pItr->IsLast())
					{
						return *this;
					}

					++pItr;
				}
				while (pItr->IsEmpty());

				return *this;
			}

			Iterator operator++(int)
			{
				Iterator temp(*this);
				++(*this);
				return *temp;
			}

			Bool8 operator==(const Iterator& it) const
			{
				return pItr == it.pItr;
			}

			Bool8 operator!=(const Iterator& it) const
			{
				return pItr != it.pItr;
			}

			Bool8 operator>=(const Iterator& it) const
			{
				return pItr >= it.pItr;
			}

			Bool8 operator<=(const Iterator& it) const
			{
				return pItr <= it.pItr;
			}

			Bool8 operator>(const Iterator& it) const
			{
				return pItr > it.pItr;
			}

			Bool8 operator<(const Iterator& it) const
			{
				return pItr < it.pItr;
			}

			KeyValueType& operator*()
			{
				return pItr->keyValue;
			}

			KeyValueType* operator->()
			{
				return &pItr->keyValue;
			}
		};

	private:
		Array<TableEntryType> mTable;
		UInt32 mSize;
		UInt32 mCapacity;
		UInt32 mThreshold;
		UInt32 mMask;

		UInt32 PrepHash(UInt32 hash)
		{
			//hash &= 0x7FFFFFFF;
			hash |= hash == 0;
			return hash;
		}

		UInt32 GetDistance(UInt32 hash, UInt32 index)
		{
			return (index + mCapacity - (hash & mMask)) & mMask;
		}

		TableEntryType* FindEntry(UInt32 hash, const KeyValueType& keyValue)
		{
			hash = PrepHash(hash);

			UInt32 index = hash & mMask;
			UInt32 original = index;
			UInt32 dist = 0;

			while (true)
			{
				if (mTable[index].hash == 0 || dist > GetDistance(mTable[index].hash, index))
				{
					return nullptr;
				}
				else if (mTable[index].hash == hash && mTable[index].keyValue == keyValue)
				{
					return &mTable[index];
				}

				index = (index + 1) & mMask;
				++dist;
			}

			return nullptr;
		}

		template<typename KeyValue>
		TableEntryType& FindInsertEntry(UInt32 hash, KeyValue&& keyValue)
		{
			hash = PrepHash(hash);

			UInt32 index = hash & mMask;
			UInt32 original = index;
			UInt32 dist = 0;

			TableEntryType entry;

			while (true)
			{
				// Entry is empty
				if (mTable[index].hash == 0)
				{
					return mTable[index];
				}

				// Found previous key entry
				if (mTable[index].keyValue == std::forward<KeyValue>(keyValue))
				{
					return mTable[index];
				}

				UInt32 currentDist = GetDistance(mTable[index].hash, index);

				if (currentDist < dist)
				{
					// Entry is erased
					if (mTable[index].isDeleted)
					{
						return mTable[index];
					}

					Swap(entry, mTable[index]);

					dist = currentDist;
				}

				index = (index + 1) & mMask;
				++dist;
			}
		}

		template<typename KeyValue>
		KeyValueType& InsertImpl(UInt32 hash, KeyValue&& keyValue)
		{
			hash = PrepHash(hash);

			UInt32 index = hash & mMask;
			UInt32 original = index;
			UInt32 dist = 0;

			TableEntryType entry(hash, std::forward<KeyValue>(keyValue));

			while (true)
			{
				// Entry is empty
				if (mTable[index].hash == 0)
				{
					++mSize;
					Swap(entry, mTable[index]);
					return mTable[index].keyValue;
				}

				// Found previous key entry
				if (mTable[index].keyValue == entry.keyValue)
				{
					Swap(entry, mTable[index]);
					return mTable[index].keyValue;
				}

				UInt32 currentDist = GetDistance(mTable[index].hash, index);

				if (currentDist < dist)
				{
					// Entry is erased
					if (mTable[index].isDeleted)
					{
						++mSize;
						Swap(entry, mTable[index]);
						return mTable[index].keyValue;
					}

					Swap(entry, mTable[index]);

					dist = currentDist;
				}

				index = (index + 1) & mMask;
				++dist;
			}
		}

		void ResizeRehash(UInt32 size)
		{
			TableType mNewTable(size);

			TableEntryType* pEntry = &mTable[0];

			while (!pEntry->IsLast())
			{
				if (!pEntry->IsEmpty())
				{
					mNewTable.Insert(pEntry->hash, pEntry->keyValue);
				}

				++pEntry;
			}

			if (!pEntry->IsEmpty())
			{
				mNewTable.Insert(pEntry->hash, pEntry->keyValue);
			}

			Swap(mNewTable, *this);
		}

	public:
		Table()
			: mTable(INITAL_SIZE, TableEntryType()), mSize(0), mCapacity(INITAL_SIZE),
			mThreshold(INITAL_SIZE * LOAD_FACTOR), mMask(INITAL_SIZE - 1)
		{
			mTable[0].isFirst = true;
			mTable[mCapacity - 1].isLast = true;
		}

		Table(UInt32 capacity)
			: mTable(capacity, TableEntryType()), mSize(0), mCapacity(capacity),
			mThreshold(capacity * LOAD_FACTOR), mMask(capacity - 1)
		{
			mTable[0].isFirst = true;
			mTable[mCapacity - 1].isLast = true;
		}

		Table(const TableType& table)
			: mTable(table.mTable), mSize(table.mSize), mCapacity(table.mCapacity),
			mThreshold(table.mThreshold), mMask(table.mMask) { }

		Table(TableType&& table) noexcept
			: Table()
		{
			Swap(*this, table);
		}

		template<typename Value>
		KeyValueType& Insert(UInt32 hash, Value&& keyValue)
		{
			if (mSize + 1 >= mThreshold)
			{
				ResizeRehash(NextGreaterPowerOf2(mCapacity));
			}
			
			return InsertImpl(hash, std::forward<Value>(keyValue));
		}

		void Remove(UInt32 hash, const KeyValueType& keyValue)
		{
			TableEntryType* pEntry = FindEntry(hash, keyValue);

			if (pEntry)
			{
				pEntry->hash = 0;
				mSize--;
			}
		}

		KeyValueType* Find(UInt32 hash, const KeyValueType& keyValue)
		{
			TableEntryType* pEntry = FindEntry(hash, keyValue);
			return pEntry == nullptr ? nullptr : &pEntry->keyValue;
		}

		Bool8 Reserve(UInt32 size)
		{
			if (size < mSize)
			{
				// Cannot resize
				return false;
			}

			ResizeRehash(NextPowerOf2(size));

			return true;
		}

		void Shrink()
		{
			UInt32 newSize = NextPowerOf2(mSize);

			if (newSize != mSize)
			{
				ResizeRehash(newSize);
			}
		}

		friend void Swap(TableType& table1, TableType& table2)
		{
			using Quartz::Swap;
			Swap(table1.mTable, table2.mTable);
			Swap(table1.mSize, table2.mSize);
			Swap(table1.mCapacity, table2.mCapacity);
			Swap(table1.mThreshold, table2.mThreshold);
			Swap(table1.mMask, table2.mMask);
		}

		TableType& operator=(TableType table)
		{
			Swap(*this, table);
			return *this;
		}

		Bool8 Contains(UInt32 hash, const KeyValueType& keyValue)
		{
			return Find(hash, keyValue) != nullptr;
		}

		void Clear()
		{
			mTable.Clear();
			mTable.Resize(mCapacity, TableEntryType());
			mSize = 0;
		}

		Iterator begin()
		{
			if (mTable.IsEmpty())
			{
				return nullptr;
			}

			Iterator it(&mTable[0]);

			if (mTable[0].IsEmpty())
			{
				return ++it;
			}

			return it;
		}

		Iterator end()
		{
			if (mTable.IsEmpty())
			{
				return nullptr;
			}

			return Iterator(&mTable[mCapacity - 1]);
		}

		/*
		const Iterator begin() const noexcept
		{
			Iterator itr(&mTable[0]);
			if (mTable[0].IsEmpty()) { ++itr; }
			return itr;
		}

		const Iterator end() const noexcept
		{
			return Iterator(&mTable[mCapacity - 1]);
		}
		*/

		TableEntryType* Data()
		{
			return mTable.Data();
		}

		UInt32 Size() const
		{
			return mSize;
		}

		UInt32 Capacity() const
		{
			return mCapacity;
		}

		UInt32 Threshold() const
		{
			return mThreshold;
		}
	};
}