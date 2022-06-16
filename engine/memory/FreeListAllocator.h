#pragma once
#include "defines.hpp"

namespace gf3d
{
	class FreeListAllocator
	{
	private:
		struct AllocationHeader
		{
			u64 blockSize = 0;
			u64 padding = 0;
		};
		struct Node
		{
			u64 blockSize = 0;
			Node* next = nullptr;
		};

	public:
		void init(u64 totalSize);
		void destroy();
		void* allocate(u64 size, u64 alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__);
		void free(void* ptr);

	private:
		Node* findNode(const u64 size, u64* padding, Node* previous, const u64 alignment);
		void insertNode(Node* previous, Node* newNode);
		void removeNode(Node* previous, Node* deleteNode);
		u64 calculatePadding(const u64 address, const u64 alignment);
		void coalescence(Node* previous, Node* freeNode);

	private:
		void* mData = nullptr;
		Node* mHead = nullptr;
		u64 mTotalSize = 0;
		u64 mUsed = 0;
	};

} //namespace gf3d