#pragma once
#include "defines.hpp"

namespace gf3d
{
	class Allocator 
	{
	public:
		virtual ~Allocator() { mTotalSize = 0; }
		virtual void init(const u64 totalSize) = 0;
		virtual void* allocate(const u64 size) = 0;
		virtual void free() = 0;
	protected:
		u64 mTotalSize = 0;
		u64 mAllocated = 0;
	};

} // namespace gf3d