#pragma once
#include "defines.hpp"
#include "core/gf3d_assert.hpp"

namespace gf3d
{
	template<class T>
	class forward_list
	{

	private:
		struct base_flist_node
		{
			base_flist_node() : pNext(nullptr) { }
			base_flist_node(const base_flist_node& other) = delete;
			base_flist_node& operator=(const base_flist_node& other) = delete;
			base_flist_node* pNext;
		};

		struct flist_node : base_flist_node
		{
			flist_node(const base_flist_node& other) = delete;
			flist_node& operator=(const base_flist_node& other) = delete;
			T data;
			void deallocate()
			{
				data.~T();
			}
		};

	public:

		class flist_iterator
		{
		public:

			flist_iterator(base_flist_node* n) noexcept
				: node(static_cast<flist_node*>(n)) { }

			T& operator*() const 
			{ 
				return static_cast<flist_node*>(node)->data; 
			}

			flist_iterator& operator++() noexcept
			{
				node = static_cast<flist_node*>(node->pNext);
				return *this;
			}

			bool operator==(const flist_iterator& other) const noexcept
			{ 
				return node == other.node; 
			}

			bool operator!=(const flist_iterator& other) const noexcept
			{
				return !(*this == other);
			}

			T* operator->() const
			{ 
				return &static_cast<flist_node*>(static_cast<void*>((node)))->data; 
			}

		public:
			flist_node* node;
		};

		typedef flist_iterator iterator;
		typedef const flist_iterator const_iterator;
		typedef base_flist_node base_node;
		typedef flist_node node;

		forward_list() noexcept;

		forward_list(u64 size) noexcept;

		forward_list(u64 size, const T& value) noexcept;

		forward_list(const forward_list& other) noexcept;
					 
		forward_list(forward_list&&) noexcept;

		~forward_list();

		void assign(iterator _begin, iterator _end);

		forward_list& operator=(const forward_list& other);
		forward_list& operator=(forward_list&& other) noexcept;

		iterator before_begin() noexcept { return iterator(&mBeforeBegin); }
		iterator begin() noexcept { return iterator(mBeforeBegin.pNext); }
		iterator begin() const noexcept { return iterator(mBeforeBegin.pNext); }
		iterator end() noexcept { return iterator(nullptr); }
		iterator end() const noexcept { return iterator(nullptr); }
		bool is_empty() const noexcept { return mBeforeBegin.pNext == nullptr; }
		T& front() { return *begin(); }
		T& front() const { return *begin(); }
		iterator insert_after(const_iterator position, T&& value) 
			{ return emplace_after(position, static_cast<T&&>(value)); }

		void pop_front();
		void clear() noexcept;

	public:

		template<class... Args>
		void _Insert_after(node* pNode, Args&&... args)
		{
			node* newNode = static_cast<node*>(::operator new(sizeof(node)));
			::new(&newNode->data) T(static_cast<Args&&>(args)...);
			newNode->pNext = mBeforeBegin.pNext;
			mBeforeBegin.pNext = newNode;
		}
		
	public:

		template<class... Args>
		T& emplace_front(Args&&... args)
		{
			node* newNode = static_cast<node*>(::operator new(sizeof(node)));
			node* beginPtr = static_cast<node*>(mBeforeBegin.pNext);
			::new(&newNode->data) T(static_cast<Args&&>(args)...);
			mBeforeBegin.pNext = newNode;
			newNode->pNext = beginPtr;
			return newNode->data;
		}

	public:

		template<class... Args>
		iterator emplace_after(const_iterator position, Args&&... args)
		{
			node* newNode = static_cast<node*>(::operator new(sizeof(node)));
			::new(&newNode->data) T(static_cast<Args&&>(args)...);
			newNode->pNext = position.node->pNext;
			position.node->pNext = newNode;
			return iterator(newNode);
		}

	public:

		template<class... Args>
		void push_front(Args&&... args)
		{
			_Insert_after(static_cast<node*>(mBeforeBegin.pNext), static_cast<Args&&>(args)...);
		}

		void push_front(const T& value)
		{
			_Insert_after(static_cast<node*>(mBeforeBegin.pNext), value);
		}

	private:
		template<class... Args>
		void append_n(u64 count, Args&&... args)
		{
			if (count <= 0) {
				return;
			}

			node* newNode = static_cast<node*>(::operator new(sizeof(node)));
			::new(&newNode->data) T(args...);
			newNode->pNext = nullptr;
			mBeforeBegin.pNext = newNode;
			--count;


			for (; count > 0; --count) {
				newNode = static_cast<node*>(::operator new(sizeof(node)));
				::new(&newNode->data) T(args...);
				newNode->pNext = mBeforeBegin.pNext;
				mBeforeBegin.pNext = newNode;
			}
		}

	private:
		void assign_move(forward_list& other) noexcept
		{
			clear();

			mBeforeBegin.pNext = other.mBeforeBegin.pNext;
			other.mBeforeBegin.pNext = nullptr;
		}

	private:

		base_node mBeforeBegin;
	};

	template<class T>
	inline forward_list<T>::forward_list() noexcept
	{

	}

	template<class T>
	inline forward_list<T>::forward_list(u64 size) noexcept
	{
		append_n(size);
	}

	template<class T>
	inline forward_list<T>::forward_list(u64 size, const T& value) noexcept
	{
		append_n(size, value);
	}

	template<class T>
	inline forward_list<T>::forward_list(const forward_list& other) noexcept
	{
		assign(other.begin(), other.end());
	}

	template<class T>
	inline forward_list<T>::forward_list(forward_list&& other) noexcept
	{
		mBeforeBegin.pNext = other.mBeforeBegin.pNext;
		other.mBeforeBegin.pNext = nullptr;
	}

	template<class T>
	inline forward_list<T>::~forward_list()
	{
		clear();
	}

	template<class T>
	inline void forward_list<T>::assign(iterator _begin, iterator _end)
	{
		node* previousNode = static_cast<node*>(&mBeforeBegin);
		for (; _begin != _end; ++_begin) { // Copies the data
			if (previousNode->pNext != nullptr) {
				static_cast<node*>(previousNode->pNext)->data = *_begin;
				previousNode = static_cast<node*>(previousNode->pNext);
				continue;
			}
			node* newNode = static_cast<node*>(::operator new(sizeof(node)));
			newNode->pNext = nullptr;
			::new(&newNode->data) T(*_begin);
			previousNode->pNext = newNode;
			previousNode = newNode;
		}

		node* orphanNode = static_cast<node*>(previousNode->pNext);
		node* nextNode;
		for (; orphanNode != nullptr;) { // Destroy all orphan nodes
			nextNode = static_cast<node*>(orphanNode->pNext);
			orphanNode->deallocate();
			::operator delete(orphanNode);
			orphanNode = nextNode;
			previousNode->pNext = nullptr;
			previousNode = orphanNode;
		}
	}

	template<class T>
	inline forward_list<T>& forward_list<T>::operator=(const forward_list<T>& other)
	{
		if (this != &other) {
			assign(other.begin(), other.end());
		}
		
		return *this;
	}

	template<class T>
	inline forward_list<T>& forward_list<T>::operator=(forward_list<T>&& other) noexcept
	{
		if (this != &other) {
			assign_move(other);
		}

		return *this;
	}

	template<class T>
	inline void forward_list<T>::pop_front()
	{
		node* nodePtr = static_cast<node*>(mBeforeBegin.pNext);
		base_node* nextPtr = nodePtr->pNext;
		nodePtr->deallocate();
		::operator delete(nodePtr);
		mBeforeBegin.pNext = nextPtr;

	}

	template<class T>
	inline void forward_list<T>::clear() noexcept
	{
		node* nextPtr;
		node* nodePtr = static_cast<node*>(mBeforeBegin.pNext);

		for (; nodePtr != nullptr; nodePtr = nextPtr) {
			nextPtr = static_cast<node*>(nodePtr->pNext);
			nodePtr->deallocate();
			::operator delete(nodePtr);
		}

		mBeforeBegin.pNext = nullptr;
	}
}