#include "core/gf3d_logger.h"
#include "FreeListAllocator.h"
#include <cstdlib>

namespace gf3d
{
	FreeListAllocator::FreeListAllocator(PlacementPolicy policy)
		: mPolicy(policy)
	{

	}

	void FreeListAllocator::init(const u64 totalSize)
	{
		if (mData != nullptr) {
			LOGGER_WARN("Allocator is already allocated, restarting the allocator");
			destroy();
		}

		mData = std::malloc(totalSize);

		if (mData) {
			mHead = reinterpret_cast<Node*>(mData);
			mHead->blockSize = totalSize;
			mHead->next = nullptr;
		}

		mTotalSize = totalSize;
	}

	void* FreeListAllocator::allocate(const u64 size, const u64 alignment)
	{
		mAllocated = size + alignment;
		void* data = findNode(size, alignment);
		return reinterpret_cast<char*>(data) + sizeof(Node);
	}

	void FreeListAllocator::free(void* ptr)
	{
		
	}

	void FreeListAllocator::destroy()
	{
		if (mData) {
			std::free(mData);
			mTotalSize = 0;
			mAllocated = 0;
		}
	}

	FreeListAllocator::Node* FreeListAllocator::findNode(const u64 size, const u64 alignment)
	{
		switch (mPolicy)
		{
		case PlacementPolicy::eFindFirst:
			return findFirstNode(size, alignment);
		case PlacementPolicy::eFindBest:
			return findBestNode(size, alignment);
		default:
			return findFirstNode(size, alignment);
		}
	}

	FreeListAllocator::Node* FreeListAllocator::findFirstNode(const u64 size, const u64 alignment)
	{
		Node* node = mHead;
		Node* previous = nullptr;
		u64 padding = 0;
		const u64 startAddress = reinterpret_cast<u64>(mData);

		while (node != nullptr) {
			u64 currentAddress = reinterpret_cast<u64>(node);
			u64 offset = startAddress - currentAddress;
			if (alignment != 0 && offset % alignment != 0) {
				padding = calculatePaddingWithHeader(currentAddress, alignment, sizeof(AllocationHeader));
			}
			u64 requireSpace = size + padding;
			node = nullptr;
		}

		return nullptr;
	}

	FreeListAllocator::Node* FreeListAllocator::findBestNode(const u64 size, const u64 alignment)
	{
		return nullptr;
	}

	void FreeListAllocator::coalescence(Node* previousNode, Node* freeNode)
	{

	}
}