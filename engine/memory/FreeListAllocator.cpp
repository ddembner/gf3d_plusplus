#include "core/gf3d_logger.h"
#include "FreeListAllocator.h"
#include "containers/forward_list.hpp"
#include <forward_list>
#include <cstdlib>

namespace gf3d
{
	void FreeListAllocator::init(u64 totalSize)
	{
		if (mData) {
			destroy();
		}

		mData = std::malloc(totalSize);

		if (!mData) {
			// Print Error Here
			return;
		}

		mTotalSize = totalSize;

		Node* newNode = static_cast<Node*>(mData);
		newNode->blockSize = totalSize;
		newNode->next = nullptr;
		insertNode(nullptr, newNode);
	}

	void FreeListAllocator::destroy()
	{
		if (mData) {
			std::free(mData);
			mUsed = 0;
			mTotalSize = 0;
			mData = nullptr;
		}
	}

	void* FreeListAllocator::allocate(u64 size, u64 alignment)
	{
		/*
			1. Find Node
			2. Check if node size is larger than requested size (split block if yes)
			3. Remove allocated node from freelist
			4. Fill out Header information
			5. Return data block address
		*/

		if (size < sizeof(Node)) {
			size = sizeof(Node);
		}

		Node* previous = nullptr;
		u64 padding = 0;
		Node* node = findNode(size, &padding, previous, alignment);

		if (!node) {
			// Print Warning here
			return nullptr;
		}

		u64 requiredSpace = size + padding;
		u64 alignmentPadding = padding - sizeof(AllocationHeader);
		u64 remaining = node->blockSize - requiredSpace;

		if (remaining > 16) { // size of node + alignment size
			Node* newNode = reinterpret_cast<Node*>(reinterpret_cast<char*>(node) + requiredSpace);
			newNode->blockSize = remaining;
			newNode->next = nullptr;
			insertNode(node, newNode);
		}

		removeNode(previous, node);

		AllocationHeader* header = reinterpret_cast<AllocationHeader*>(reinterpret_cast<char*>(node) + alignmentPadding);
		header->blockSize = requiredSpace;
		header->padding = alignmentPadding;

		mUsed += header->blockSize;

		return reinterpret_cast<void*>(reinterpret_cast<char*>(header) + sizeof(AllocationHeader));
	}

	void FreeListAllocator::free(void* ptr)
	{
		if (!ptr) {
			return;
		}

		if (ptr < mData || reinterpret_cast<u64>(ptr) > reinterpret_cast<u64>(mData) + mTotalSize) {
#ifndef NDEBUG
			LOGGER_WARN("Freeing a pointer that was not part of this system");
#endif
			
			std::free(ptr);
			return;
		}

		AllocationHeader* header = reinterpret_cast<AllocationHeader*>(static_cast<char*>(ptr) - sizeof(AllocationHeader));

		Node* freeNode = reinterpret_cast<Node*>(reinterpret_cast<char*>(header) - header->padding);
		freeNode->blockSize = header->blockSize;
		freeNode->next = nullptr;

		Node* node = mHead;
		Node* previous = nullptr;

		while (node) {
			if (ptr < node) {
				insertNode(previous, freeNode);
				break;
			}

			previous = node;
			node = node->next;
		}

		mUsed -= freeNode->blockSize;

		coalescence(previous, freeNode);

	}

	FreeListAllocator::Node* FreeListAllocator::findNode(const u64 size, u64* padding, Node* previous, const u64 alignment)
	{
		Node* node = mHead;

		while (node) {
			*padding = calculatePadding(reinterpret_cast<u64>(node), alignment);

			if (node->blockSize >= size + *padding) {
				break;
			}

			previous = node;
			node = node->next;
		}

		return node;
	}

	void FreeListAllocator::insertNode(Node* previous, Node* newNode)
	{
		if (!previous) {
			// This is first node

			if (mHead) {
				newNode->next = mHead;
			}
			else {
				newNode->next = nullptr;
			}

			mHead = newNode;
			return;
		}

		if (!previous->next) {
			// This is last node
			previous->next = newNode;
			return;
		}

		// This is a middle node
		newNode->next = previous->next;
		previous->next = newNode;
	}

	void FreeListAllocator::removeNode(Node* previous, Node* deleteNode)
	{
		if (!previous) {

			if (!deleteNode->next) {
				mHead = nullptr;
			}
			else {
				mHead = deleteNode->next;
			}

			return;
		}

		previous->next = deleteNode->next;
	}

	u64 FreeListAllocator::calculatePadding(const u64 address, const u64 alignment)
	{
		u64 padding = address % alignment;
		u64 neededSpace = sizeof(AllocationHeader);

		return padding + neededSpace;
	}

	void FreeListAllocator::coalescence(Node* previous, Node* freeNode)
	{
		u64 freeNodeNum = reinterpret_cast<u64>(freeNode);
		u64 freeNodeNextNum = reinterpret_cast<u64>(freeNode->next);
		u64 previousNum = reinterpret_cast<u64>(previous);
		if (freeNode->next && reinterpret_cast<u64>(freeNode) + freeNode->blockSize == reinterpret_cast<u64>(freeNode->next)) {
			freeNode->blockSize += freeNode->next->blockSize;
			removeNode(freeNode, freeNode->next);
		}


		if (previous && reinterpret_cast<u64>(previous) + previous->blockSize == reinterpret_cast<u64>(freeNode)) {
			previous->blockSize += freeNode->blockSize;
			removeNode(previous, freeNode);
		}
	}
} // namespace gf3d