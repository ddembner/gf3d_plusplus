#include "test_vector.hpp"
#include "test_macros.hpp"
#include <containers/vector.hpp>
#include <vector> //Test against std::vector

u8 VectorCreateAndDestroy()
{
	std::vector<u32> stdNumbersEmpty;
	gf3d::vector<u32> gf3dNumbersEmpty;

	should_be_equal(reinterpret_cast<void*>(stdNumbersEmpty.data()), reinterpret_cast<void*>(gf3dNumbersEmpty.data()));
	should_be_equal(stdNumbersEmpty.capacity(), gf3dNumbersEmpty.capacity());
	should_be_equal(stdNumbersEmpty.size(), gf3dNumbersEmpty.size());
	should_be_equal(0, gf3dNumbersEmpty.capacity());
	should_be_equal(0, gf3dNumbersEmpty.size());

	std::vector<u32> stdNumbers(3);
	gf3d::vector<u32> gf3dNumbers(3);

	should_not_be_equal(reinterpret_cast<void*>(stdNumbers.data()), reinterpret_cast<void*>(gf3dNumbers.data()));
	should_not_be_equal(nullptr, reinterpret_cast<void*>(gf3dNumbers.data()));
	should_be_equal(stdNumbers.capacity(), gf3dNumbers.capacity());
	should_be_equal(stdNumbers.size(), gf3dNumbers.size());

	stdNumbers.~vector();
	gf3dNumbers.~vector();

	should_be_equal(reinterpret_cast<void*>(stdNumbers.data()), reinterpret_cast<void*>(gf3dNumbers.data()));
	should_be_equal(nullptr, reinterpret_cast<void*>(gf3dNumbers.data()));
	should_be_equal(stdNumbers.capacity(), gf3dNumbers.capacity());
	should_be_equal(stdNumbers.size(), gf3dNumbers.size());
	should_be_equal(0, gf3dNumbers.capacity());
	should_be_equal(0, gf3dNumbers.size());

	return TEST_PASS;
}

void RegisterVectorTests(TestManager& manager)
{
	manager.registerTest(VectorCreateAndDestroy, "vector create and destroy");
}