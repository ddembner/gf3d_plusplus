#pragma once
#include "Allocator.hpp"

namespace gf3d
{
	class LinearAllocator : public Allocator
	{
	public:
		~LinearAllocator();
		/// <summary>
		/// Initializes the allocator with a maximum capacity
		/// </summary>
		/// <param name="size"> The total size </param>
		void init(const u64 totalSize) override;

		void* allocate(const u64 size, const u64 alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__) override;

		void free(void* ptr) override;

		/// <summary>
		/// Sets the total memory allocated from this allocator to 0 and sets the memory contents to 0
		/// </summary>
		void reset();

	private:
		void* mPtr = nullptr;
	};

}