#pragma once
#include "defines.hpp"

namespace gf3d
{
	template<class T>
	class forward_list
	{

	private:
		struct base_flist_node
		{
			base_flist_node() : pNext(nullptr) { }
			base_flist_node* pNext;
		};

		struct flist_node : base_flist_node
		{
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
				: node(n) { }

			T& operator*() const 
			{ 
				return static_cast<flist_node*>(node)->data; 
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

		private:
			base_flist_node* node;
		};

		typedef flist_iterator iterator;
		typedef base_flist_node base_node;
		typedef flist_node node;

		forward_list() noexcept;

		forward_list(u64 size) noexcept;

		forward_list(u64 size, const T& value) noexcept;

		~forward_list();

		iterator before_begin() noexcept { return iterator(&mBeforeBegin); }
		iterator begin() noexcept { return iterator(mBeforeBegin.pNext); }

		iterator end() noexcept { return iterator(nullptr); }

		void clear() noexcept;

	private:
		template<class... Args>
		void append_n(u64 count, const Args&... args)
		{
			if (count <= 0) {
				return;
			}

			node* newNode = new node;
			::new(&newNode->data) T(static_cast<Args&&>(args)...);
			mBeforeBegin.pNext = newNode;
			--count;


			for (; count > 0; --count) {
				newNode = new node;
				::new(&newNode->data) T(static_cast<Args&&>(args)...);
				newNode->pNext = mBeforeBegin.pNext;
				mBeforeBegin.pNext = newNode;
			}
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
		if (size > 0) {
			

			for (u64 i = 0; i < size; i++) {
				
			}
		}
	}

	template<class T>
	inline forward_list<T>::~forward_list()
	{
		clear();
	}

	template<class T>
	inline void forward_list<T>::clear() noexcept
	{
		node* nextPtr;
		node* nodePtr = static_cast<flist_node*>(mBeforeBegin.pNext);

		for (; nodePtr != nullptr; nodePtr = nextPtr) {
			nextPtr = static_cast<flist_node*>(nodePtr->pNext);
			nodePtr->deallocate();
			::operator delete(nodePtr);
		}
	}
}