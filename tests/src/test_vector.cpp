#include "test_vector.hpp"
#include "test_macros.hpp"
#include <containers/vector.hpp>
#include <vector> //Test against std::vector

struct TestObject
{
	u32 x, y, z;

	TestObject() : x(0), y(1), z(2) {
		// Default
	}

	~TestObject() {
		// Destructor
	}
};

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

u8 VectorIndex()
{
	std::vector<u32> stdNumbers(3);
	gf3d::vector<u32> gf3dNumbers(3);

	stdNumbers[0] = 5;
	gf3dNumbers[0] = 5;

	should_be_equal(5, gf3dNumbers[0]);
	should_be_equal(stdNumbers[0], gf3dNumbers[0]);

	stdNumbers[1] = 7;
	gf3dNumbers[1] = 9;

	should_be_equal(9, gf3dNumbers[1]);
	should_not_be_equal(stdNumbers[1], gf3dNumbers[1]);

	return TEST_PASS;
}

u8 VectorObjectConstruct()
{
	gf3d::vector<TestObject> gf3dNumbers(3);

	should_be_equal(0, gf3dNumbers[0].x);
	should_be_equal(1, gf3dNumbers[0].y);
	should_be_equal(2, gf3dNumbers[0].z);

	should_be_equal(0, gf3dNumbers[1].x);
	should_be_equal(1, gf3dNumbers[1].y);
	should_be_equal(2, gf3dNumbers[1].z);

	should_be_equal(0, gf3dNumbers[2].x);
	should_be_equal(1, gf3dNumbers[2].y);
	should_be_equal(2, gf3dNumbers[2].z);

	return TEST_PASS;
}

u8 VectorReserve()
{
	std::vector<TestObject> stdNumbers;
	gf3d::vector<TestObject> gf3dNumbers;

	stdNumbers.reserve(5);
	gf3dNumbers.reserve(5);

	should_be_equal(5, gf3dNumbers.capacity());
	should_not_be_equal(5, gf3dNumbers.size());
	should_not_be_equal(nullptr, reinterpret_cast<void*>(gf3dNumbers.data()));

	return TEST_PASS;
}

void RegisterVectorTests(TestManager& manager)
{
	manager.registerTest(VectorCreateAndDestroy, "vector create and destroy");
	manager.registerTest(VectorIndex, "vector c array like indexing");
	manager.registerTest(VectorObjectConstruct, "vector using object constructor and destructor");
	manager.registerTest(VectorReserve, "vector reserve");
}