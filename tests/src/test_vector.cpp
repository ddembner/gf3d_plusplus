#include "test_vector.hpp"
#include "test_macros.hpp"
#include <containers/vector.hpp>
#include <vector> //Test against std::vector
#include "test_object.hpp"

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

	gf3dNumbers.~vector();

	should_be_equal(nullptr, reinterpret_cast<void*>(gf3dNumbers.data()));

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
	//Testing default constructor
	gf3d::vector<TestObject> gf3dNumbers(3);

	should_be_equal(0, gf3dNumbers[0].x);
	should_be_equal(0, gf3dNumbers[0].y);
	should_be_equal(0, gf3dNumbers[0].z);

	should_be_equal(0, gf3dNumbers[1].x);
	should_be_equal(0, gf3dNumbers[1].y);
	should_be_equal(0, gf3dNumbers[1].z);

	should_be_equal(0, gf3dNumbers[2].x);
	should_be_equal(0, gf3dNumbers[2].y);
	should_be_equal(0, gf3dNumbers[2].z);

	//Testing copy constructor
	gf3d::vector<TestObject> gf3dNumbers2(3, TestObject(4));

	should_be_equal(4, gf3dNumbers2[0].x);
	should_be_equal(4, gf3dNumbers2[0].y);
	should_be_equal(4, gf3dNumbers2[0].z);

	should_be_equal(4, gf3dNumbers2[1].x);
	should_be_equal(4, gf3dNumbers2[1].y);
	should_be_equal(4, gf3dNumbers2[1].z);

	should_be_equal(4, gf3dNumbers2[2].x);
	should_be_equal(4, gf3dNumbers2[2].y);
	should_be_equal(4, gf3dNumbers2[2].z);

	return TEST_PASS;
}

u8 VectorReserve()
{
	std::vector<TestObject> stdNumbers;
	gf3d::vector<TestObject> gf3dNumbers;

	gf3dNumbers.emplace_back();
	gf3dNumbers.emplace_back(4);
	gf3dNumbers.emplace_back(1, 2, 3);

	should_be_equal(3, gf3dNumbers.capacity());
	should_be_equal(3, gf3dNumbers.size());
	should_not_be_equal(nullptr, reinterpret_cast<void*>(gf3dNumbers.data()));

	gf3dNumbers.reserve(7);

	should_be_equal(7, gf3dNumbers.capacity());
	should_be_equal(3, gf3dNumbers.size());
	
	should_be_equal(0, gf3dNumbers[0].x);
	should_be_equal(0, gf3dNumbers[0].y);
	should_be_equal(0, gf3dNumbers[0].z);
	
	should_be_equal(4, gf3dNumbers[1].x);
	should_be_equal(4, gf3dNumbers[1].y);
	should_be_equal(4, gf3dNumbers[1].z);

	should_be_equal(1, gf3dNumbers[2].x);
	should_be_equal(2, gf3dNumbers[2].y);
	should_be_equal(3, gf3dNumbers[2].z);

	gf3dNumbers.emplace_back(4, 5, 6);

	should_be_equal(7, gf3dNumbers.capacity());
	should_be_equal(4, gf3dNumbers.size());

	should_be_equal(4, gf3dNumbers[3].x);
	should_be_equal(5, gf3dNumbers[3].y);
	should_be_equal(6, gf3dNumbers[3].z);

	gf3dNumbers.reserve(0);

	should_not_be_equal(0, gf3dNumbers.capacity());
	should_not_be_equal(0, gf3dNumbers.size());
	should_be_equal(4, gf3dNumbers.size());


	return TEST_PASS;
}

u8 VectorPopBack()
{
	gf3d::vector<TestObject> gf3dNumbers(3);

	gf3dNumbers.pop_back();
	gf3dNumbers.pop_back();

	should_be_equal(3, gf3dNumbers.capacity());
	should_not_be_equal(3, gf3dNumbers.size());
	should_be_equal(1, gf3dNumbers.size());

	return TEST_PASS;
}

u8 VectorShrinkToFit()
{
	gf3d::vector<TestObject> gf3dNumbers;
	gf3dNumbers.reserve(5);
	gf3dNumbers.emplace_back();
	gf3dNumbers.emplace_back();
	gf3dNumbers.emplace_back();
	gf3dNumbers.shrink_to_fit();

	should_not_be_equal(5, gf3dNumbers.capacity());
	should_be_equal(3, gf3dNumbers.size());
	should_be_equal(3, gf3dNumbers.capacity());

	return TEST_PASS;
}

u8 VectorResize()
{
	gf3d::vector<TestObject> gf3dNumbers;
	std::vector<TestObject> stdNumbers;

	gf3dNumbers.resize(5);
	
	should_be_equal(5, gf3dNumbers.size());
	should_be_equal(5, gf3dNumbers.capacity());

	gf3dNumbers.resize(3);
	
	should_be_equal(3, gf3dNumbers.size());
	should_be_equal(3, gf3dNumbers.capacity());
	
	return TEST_PASS;
}

u8 VectorSwap()
{
	gf3d::vector<TestObject> gf3dNumbersA(3, TestObject(1, 2, 3));
	gf3d::vector<TestObject> gf3dNumbersB(5, TestObject(4));

	gf3dNumbersA.swap(gf3dNumbersB);

	should_be_equal(5, gf3dNumbersA.size());
	should_be_equal(3, gf3dNumbersB.size());
	should_be_equal(5, gf3dNumbersA.capacity());
	should_be_equal(3, gf3dNumbersB.capacity());
	should_not_be_equal(reinterpret_cast<void*>(gf3dNumbersA.data()), reinterpret_cast<void*>(gf3dNumbersB.data()));
	
	return TEST_PASS;
}

u8 VectorForLoop()
{
	gf3d::vector<int> nums(5);
	for (u64 i = 0; i < nums.size(); i++)
		nums[i] = i;

	for (auto& i : nums)
		should_be_equal(i, *(nums.begin() + i));

	return TEST_PASS;
}

u8 VectorCopyAndMove()
{
	gf3d::vector<TestObject> srcVector;
	gf3d::vector<TestObject> destVector;
	
	srcVector.emplace_back();
	srcVector.emplace_back(3);
	srcVector.emplace_back(4, 5, 6);
	srcVector.emplace_back(4, 5, 6);
	srcVector.emplace_back(4, 5, 6);
	srcVector.emplace_back(4, 5, 6);
	srcVector.emplace_back(4, 5, 6);

	destVector = srcVector;

	should_be_equal(7, destVector.size());
	should_be_equal(7, destVector.capacity());

	gf3d::vector<TestObject> srcVector2(3, TestObject(1, 2, 3));
	gf3d::vector<TestObject> destVector2(4);

	destVector2 = static_cast<gf3d::vector<TestObject>&&>(srcVector2);

	should_be_equal(3, destVector2.size());
	should_be_equal(3, destVector2.capacity());
	expect_to_be_true(srcVector2.data() == nullptr);

	return TEST_PASS;
}

void RegisterVectorTests(TestManager& manager)
{
	manager.registerTest(VectorCreateAndDestroy, "vector create and destroy");
	manager.registerTest(VectorIndex, "vector c array like indexing");
	manager.registerTest(VectorObjectConstruct, "vector using object constructor and destructor");
	manager.registerTest(VectorReserve, "vector reserve");
	manager.registerTest(VectorPopBack, "vector pop back");
	manager.registerTest(VectorShrinkToFit, "vector shrink to fit");
	manager.registerTest(VectorResize, "vector resize");
	manager.registerTest(VectorSwap, "vector swap");
	manager.registerTest(VectorForLoop, "vector for loop");
	manager.registerTest(VectorCopyAndMove, "vector copy and move");
}