#include "LinearAllocator.hpp"
#include "core/gf3d_logger.h"

gf3d::LinearAllocator::~LinearAllocator()
{
	if (mPtr != nullptr) {
		LOGGER_WARN("Allocator went out of scope without being freed. Destructor has freed the memory of size {0}", mTotalSize);
		this->deallocate();
	}
}

void gf3d::LinearAllocator::init(const u64 totalSize)
{
	if (mPtr != nullptr) {
		std::free(mPtr);
	}

	mPtr = std::malloc(totalSize);
	mTotalSize = totalSize;
}

void* gf3d::LinearAllocator::allocate(const u64 size)
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

void gf3d::LinearAllocator::deallocate()
{
	std::free(mPtr);
	mPtr = nullptr;
	mAllocated = 0;
	mTotalSize = 0;
}

void gf3d::LinearAllocator::reset()
{
	if (mPtr) {
		mAllocated = 0;
		std::memset(mPtr, 0, mTotalSize);
	}
}