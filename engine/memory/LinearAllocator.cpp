#include "LinearAllocator.hpp"
#include "core/gf3d_logger.h"
#include "core/gf3d_memory.h"

gf3d::LinearAllocator::~LinearAllocator()
{
	if (mPtr != nullptr) {
		std::free(mPtr);
	}
}

void gf3d::LinearAllocator::init(const u64 totalSize)
{
	if (mPtr != nullptr) {
		delete mPtr;
	}

	mPtr = ::operator new(totalSize);
	mTotalSize = totalSize;
}

void* gf3d::LinearAllocator::allocate(const u64 size, const u64 alignment)
{
	if (mPtr) {
		if (mAllocated + size > mTotalSize) {
			u64 remaining = mTotalSize - mAllocated;
			LOGGER_ERROR("Not enough memory available - requested: {0}, remaining: {1}", size, remaining);
			return nullptr;
		}

		void* allocation = ((u64*)mPtr) + mAllocated;
		mAllocated += size;

		return allocation;
	}

	LOGGER_ERROR("Allocator has not been initialized.");
	return nullptr;
}

void gf3d::LinearAllocator::free(void* ptr)
{
	
}

void gf3d::LinearAllocator::reset()
{
	if (mPtr) {
		mAllocated = 0;
		std::memset(mPtr, 0, mTotalSize);
	}
}