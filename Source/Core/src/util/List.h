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
			ListNode* pNext;

			ListNode(Type&& value, ListNode* pNext);
		};

		ListNode* mpHead;
		ListNode* mpTail;

	public:
		List();
		List(List& list);

		void PushBack(Type&& type);
		void PushFront(Type&& type);
	};

	template<typename Type>
	List<Type>::ListNode::ListNode(Type&& value, ListNode* pNext)
		: value(value), pNext(pNext)
	{
		// Nothing
	}

	template<typename Type>
	List<Type>::List()
		: mpHead(nullptr)
	{
		// Nothing
	}

	template<typename Type>
	List<Type>::List(List& list)
		: mpHead(list.mpHead)
	{
		// Nothing
	}

	template<typename Type>
	void List<Type>::PushBack(Type&& type)
	{
		ListNode* pNewNode = new ListNode(std::forward<Type>(value), nullptr);
		mpTail->pNext = pNewNode;
		mpTail = pNewNode;
	}

	template<typename Type>
	void List<Type>::PushFront(Type&& type)
	{
		ListNode* pNewNode = new ListNode(std::forward<Type>(value), mpHead);
		mpHead = pNewNode;
	}
}