#pragma once

namespace gf3d 
{
	template<class T>
	struct StackList
	{
		struct Node
		{
			T data;
			Node* next;
		};

		Node* head;

		StackList(StackList&) = delete;
		Node* pop();
		void push(Node* node);
	};

	template<class T>
	StackList<T>::Node* StackList<T>::pop()
	{
		Node* top = head;
		Node* head = head->next;
		return top;
	}

	template<class T>
	void StackList<T>::push(Node* newNode)
	{
		newNode->next = head;
		head = newNode;
	}
}