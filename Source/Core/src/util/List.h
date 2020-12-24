#pragma once

namespace Quartz
{
	template<typename Type>
	class List
	{
	private:
		struct ListNode
		{
			Type value;
			ListNode* pLast;
			ListNode* pNext;

			ListNode(const Type& value, ListNode* pLast, ListNode* pNext)
				: value(value), pLast(pLast), pNext(pNext) { }

			ListNode(Type&& value, ListNode* pLast, ListNode* pNext)
				: value(Move(value)), pLast(pLast), pNext(pNext) { }
		};

		ListNode* mpHead;
		ListNode* mpTail;

	public:
		List()
			: mpHead(nullptr) {}

		List(const List& list)
			: mpHead(list.mpHead) {}

		void PushBack(const Type& value)
		{
			ListNode* pNewNode = new ListNode(value, mpTail, nullptr);

			if (mpTail != nullptr)
			{
				mpTail->pNext = pNewNode;
			}

			mpTail = pNewNode;
		}

		void PushBack(Type&& value)
		{
			ListNode* pNewNode = new ListNode(Move(value), mpTail, nullptr);

			if (mpTail != nullptr)
			{
				mpTail->pNext = pNewNode;
			}

			mpTail = pNewNode;
		}

		void PushFront(const Type& value)
		{
			ListNode* pNewNode = new ListNode(value, nullptr, mpHead);

			if (mpHead != nullptr)
			{
				mpHead->pLast = pNewNode;
			}

			mpHead = pNewNode;
		}

		void PushFront(Type&& value)
		{
			ListNode* pNewNode = new ListNode(Move(value), nullptr, mpHead);

			if (mpHead != nullptr)
			{
				mpHead->pLast = pNewNode;
			}

			mpHead = pNewNode;
		}

		void PopBack()
		{
			if (mpTail != nullptr)
			{
				ListNode* pOldTail = mpTail;
				mpTail = mpTail->pLast;

				if (mpTail != nullptr)
				{
					mpTail->pNext = nullptr;
				}

				delete pOldHead;
			}
		}

		void PopFront()
		{
			if (mpHead != nullptr)
			{
				ListNode* pOldHead = mpHead;
				mpHead = mpHead->pNext;

				if (mpHead != nullptr)
				{
					mpHead->pLast = nullptr;
				}

				delete pOldHead;
			}
		}

		Type* GetHead()
		{
			return mpHead != nullptr ? &mpHead->value : nullptr;
		}

		Type* GetTail()
		{
			return mpTail != nullptr ? &mpTail->value : nullptr;
		}
	};
}