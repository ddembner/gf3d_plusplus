#include "LinearAllocator.hpp"
#include "core/gf3d_logger.h"
#include "core/gf3d_memory.h"

gf3d::LinearAllocator::~LinearAllocator()
{
	if (mPtr != nullptr) {
		delete mPtr;
		mPtr = nullptr;
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

		u64 currentAddress = reinterpret_cast<u64>(mPtr) + mAllocated;

		u64 padding = 0;

		if (alignment != 0 && currentAddress % alignment != 0) {
			padding = calculatePadding(currentAddress, alignment);
		}

		u64 allocation = reinterpret_cast<u64>(mPtr) + mAllocated + padding;
		mAllocated += size + padding;

		return reinterpret_cast<void*>(allocation);
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