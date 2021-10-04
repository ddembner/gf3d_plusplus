#include "test_linear_allocator.hpp"
#include "test_manager.hpp"
#include "test_macros.hpp"
#include <memory/LinearAllocator.hpp>

u8 LinearCreateAndDestroy()
{
	gf3d::LinearAllocator allocator;

	const u64 size = sizeof(u64);

	allocator.init(size);

	void* memory = allocator.allocate(size);

	should_not_be_equal(nullptr, memory);
	should_be_equal(size, allocator.totalAllocated());
	should_be_equal(size, allocator.totalSize());

	allocator.~LinearAllocator();

	should_be_equal(0, allocator.totalAllocated());
	should_be_equal(0, allocator.totalSize());

	return TEST_PASS;
}

u8 MaxAllocate()
{
	gf3d::LinearAllocator allocator;

	const u64 size = sizeof(u64);

	allocator.init(size);

	void* memory = allocator.allocate(size);

	should_not_be_equal(nullptr, memory);
	should_be_equal(size, allocator.totalAllocated());
	should_be_equal(size, allocator.totalSize());

	return TEST_PASS;
}

u8 MaxMultiAllocate()
{
	gf3d::LinearAllocator allocator;

	const u64 size = sizeof(u64);
	const u64 maxAllocations = 1024;

	allocator.init(size * maxAllocations);

	for (u64 i = 0; i < maxAllocations; i++) {
		void* memory = allocator.allocate(size);
		should_not_be_equal(nullptr, memory);
		should_be_equal(size * (i + 1), allocator.totalAllocated());
	}

	return TEST_PASS;
}

u8 OverAllocateMulti()
{
	gf3d::LinearAllocator allocator;

	const u64 size = sizeof(u64);
	const u64 maxAllocations = 4;

	allocator.init(size * maxAllocations);

	for (u64 i = 0; i < maxAllocations; i++) {
		void* memory = allocator.allocate(size);
		should_not_be_equal(nullptr, memory);
		should_be_equal(size * (i + 1), allocator.totalAllocated());
	}

	LOGGER_DEBUG("This Test is supposed to proc an error");
	void* memory = allocator.allocate(size);
	should_be_equal(nullptr, memory);
	should_be_equal(size * maxAllocations, allocator.totalAllocated());

	return TEST_PASS;
}

u8 MultiAllocateAndFree()
{
	gf3d::LinearAllocator allocator;

	const u64 size = sizeof(u64);
	const u64 maxAllocations = 4;

	allocator.init(size * maxAllocations);

	for (u64 i = 0; i < maxAllocations; i++) {
		void* memory = allocator.allocate(size);
		should_not_be_equal(nullptr, memory);
		should_be_equal(size * (i + 1), allocator.totalAllocated());
	}

	allocator.reset();

	should_be_equal(0, allocator.totalAllocated());

	return TEST_PASS;
}

void RegisterLinearTests(TestManager& manager)
{
	manager.registerTest(LinearCreateAndDestroy, "Test  the creation and destruction of allocator");
	manager.registerTest(MaxAllocate, "Allocate maximum in a single allocation");
	manager.registerTest(MaxMultiAllocate, "Allocate maximum in a multiple allocations");
	manager.registerTest(OverAllocateMulti, "Allocating too much memory");
	manager.registerTest(MultiAllocateAndFree, "Allocating multiple times and checking if free works");
}