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
		};

	public:

		class flist_iterator
		{
		public:

			flist_iterator(base_flist_node* n) noexcept
				: node(n) { }

			T& operator*() const { return static_cast<flist_node*>(node)->data; }

			friend bool operator==(const base_flist_node& a, const base_flist_node& b)
				{ return a.node == b.node; } 

			friend bool operator!=(const base_flist_node& a, const base_flist_node& b)
				{ return !(a.node == b.node); } 

			T* operator->() const
				{ return &static_cast<flist_node*>(static_cast<void*>((node)))->data; }

		private:
			base_flist_node* node;
		};

		typedef flist_iterator iterator;
		typedef base_flist_node base_node;
		typedef flist_node node;

		forward_list() noexcept = default;

		forward_list(u64 size);

		forward_list(u64 size, const T& value);

		~forward_list();

		iterator before_begin() noexcept { return iterator(&mBeforeBegin); }
		iterator begin() noexcept { return iterator(mBeforeBegin->pNext); }

		iterator end() noexcept { return iterator(nullptr); }

	private:

		base_node mBeforeBegin;
	};

	template<class T>
	inline forward_list<T>::forward_list(u64 size)
	{
		
	}

	template<class T>
	inline forward_list<T>::forward_list(u64 size, const T& value)
	{
		if (size > 0) {
			node* mHead = reinterpret_cast<node*>(::operator new(sizeof(node) * size)); // allocate memory

			for (u64 i = 0; i < size; i++) {
				
			}
		}
	}

	template<class T>
	inline forward_list<T>::~forward_list()
	{

	}
}