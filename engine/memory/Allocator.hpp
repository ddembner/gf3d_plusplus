#pragma once
#include "defines.hpp"

namespace gf3d
{
	class Allocator 
	{
	public:
		virtual ~Allocator() { mTotalSize = 0; }
		virtual void init(const u64 totalSize) = 0;
		virtual void* allocate(const u64 size, const u64 alignment) = 0;
		virtual void free(void* ptr) = 0;
		u64 totalAllocated() const { return mAllocated; }
		u64 totalSize() const { return mTotalSize; }
	protected:
		u64 calculatePadding(const u64 baseAddress, const u64 alignment)
		{
			const u64 multiplier = (baseAddress / alignment) + 1;
			const u64 alignedAddress = multiplier * alignment;
			return alignedAddress - baseAddress;
		}

		u64 calculatePaddingWithHeader(const u64 baseAddress, const u64 alignment, u64 headerSize)
		{
			u64 padding = calculatePadding(baseAddress, alignment);
			u64 neededSpace = headerSize;

			if (padding < neededSpace) {
				// Header does not fit - Calculate next aligned address that header fits
				neededSpace -= padding;

				// How many alignments I need to fit the header        
				if (neededSpace % alignment > 0) {
					padding += alignment * (1 + (neededSpace / alignment));
				}
				else {
					padding += alignment * (neededSpace / alignment);
				}
			}

			return padding;
		}
	protected:
		u64 mTotalSize = 0;
		u64 mAllocated = 0;
	};

} // namespace gf3d