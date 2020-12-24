#pragma once

#include "../Common.h"

namespace Quartz
{
	template<typename _Type>
	class Heap
	{
	public:
		using Type = _Type;
		using HeapType = Heap<Type>;

	private:
		struct HeapNode
		{
			USize key;
			Type value;
			HeapNode* pChild;
			HeapNode* pNext;

			HeapNode(USize key, const Type& value, HeapNode* pChild, HeapNode* pNext)
				: key(key), value(value), pChild(pChild), pNext(pNext) {}

			HeapNode(USize key, Type&& value, HeapNode* pChild, HeapNode* pNext)
				: key(key), value(Move(value)), pChild(pChild), pNext(pNext) {}

			void AddChild(HeapNode* pNode)
			{
				if (pChild == nullptr)
				{
					pChild = pNode;
				}
				else
				{
					pNode->pNext = pChild;
					pChild = pNode;
				}
			}
		};

		HeapNode* mpRoot;
		USize mSize;

	private:
		HeapNode* MergeNodes(HeapNode* pNode1, HeapNode* pNode2)
		{
			if (pNode1 == nullptr)
			{
				return pNode2;
			}

			if (pNode2 == nullptr)
			{
				return pNode1;
			}

			if (pNode1->key < pNode2->key)
			{
				pNode1->AddChild(pNode2);
				return pNode1;
			}
			else
			{
				pNode2->AddChild(pNode1);
				return pNode2;
			}
		}

		HeapNode* MergeChild(HeapNode* pNode)
		{
			if (pNode == nullptr || pNode->pNext == nullptr)
			{
				return pNode;
			}

			HeapNode* pNewNode = pNode->pNext->pNext;
			HeapNode* pCurrent = pNode;
			HeapNode* pNext = pNode->pNext;

			pNode->pNext->pNext = nullptr;
			pNode->pNext = nullptr;

			return MergeNodes(MergeNodes(pCurrent, pNext), MergeChild(pNewNode));
		}

	public:
		Heap()
			: mpRoot(nullptr) {}

		/*
		Heap(const HeapType& heap)
			: mpRoot(nullptr) {}
		*/

		Type* Insert(USize key, const Type& value)
		{
			HeapNode* pNewNode = new HeapNode(key, value, nullptr, nullptr);
			mpRoot = MergeNodes(mpRoot, pNewNode);
			++mSize;

			return &pNewNode->value;
		}

		Type* Peek()
		{
			return mpRoot == nullptr ? nullptr : &mpRoot->value;
		}

		void Pop()
		{
			if (mpRoot != nullptr)
			{
				HeapNode* pPrevRoot = mpRoot;
				mpRoot = MergeChild(mpRoot->pChild);
				delete pPrevRoot;

				--mSize;
			}
		}

		void Merge(const HeapType& heap)
		{
			mpRoot = MergeNodes(mpRoot, heap.mpRoot);
			mSize += heap.mSize;
		}

		USize Size() const
		{
			return mSize;
		}

		Bool8 IsEmpty() const
		{
			return mpRoot == nullptr;
		}
	};
}