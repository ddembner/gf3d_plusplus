#pragma once
#include "defines.hpp"

namespace gf3d
{
	template<typename T>
	struct flist_node
	{
		T* mData = nullptr;
		T* mNext = nullptr;
	};

	template <class T>
	class flist_iterator
	{
	public:

	private:
		
	};


	template<class T>
	class forward_list
	{
	public:
		using iterator = flist_iterator<T>;

		forward_list() = default;

		forward_list(u64 size)
		{

		}

		~forward_list()
		{

		}

	private:
		flist_node<T> mFront;
	};
}