#pragma once
#include "Allocator.hpp"

namespace gf3d
{
	class FreeListAllocator : public Allocator
	{
	public:
		enum class PlacementPolicy
		{
			eFindFirst,
			eFindBest
		};
	public:
		FreeListAllocator(PlacementPolicy policy = PlacementPolicy::eFindFirst);

		void init(const u64 totalSize) override;

		void* allocate(const u64 size, const u64 alignment = 8) override;

		void free(void* ptr) override;

		void destroy();

	private:
		struct AllocationHeader
		{
			u64 blockSize;
			u8 padding;
		};

		struct Node
		{
			u64 blockSize;
			Node* next;
		};

	private:
		Node* findNode(const u64 size, const u64 alignment);
		Node* findFirstNode(const u64 size, const u64 alignment);
		Node* findBestNode(const u64 size, const u64 alignment);
		void coalescence(Node* previousNode, Node* freeNode);
	private:
		void* mData = nullptr;
		Node* mHead = nullptr;
		PlacementPolicy mPolicy;
	};

} //namespace gf3d