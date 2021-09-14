#pragma once
#include "Allocator.hpp"
#include "containers/stack_list.hpp"

namespace gf3d 
{
	class PoolAllocator : public Allocator
	{
	public:
		void init(const u64 totalSize) override;

		void* allocate(const u64 size) override;

		void deallocate() override;

	private:

	};
}