#include "test_vector.hpp"
#include "test_macros.hpp"
#include <containers/vector.hpp>
#include <vector> //Test against std::vector

struct TestObject
{
	u32 x, y, z;
	int* pNum = nullptr;

	TestObject() : x(0), y(0), z(0) 
	{
		pNum = new int;
		// Default
		// LOGGER_DEBUG("Default");
	}

	TestObject(u32 scalar) : x(scalar), y(scalar), z(scalar) 
	{
		pNum = new int;
		// LOGGER_DEBUG("Scalar");
	}

	TestObject(u32 _x, u32 _y, u32 _z) : x(_x), y(_y), z(_z) 
	{
		pNum = new int;
		// LOGGER_DEBUG("Construct");
	}


	TestObject(const TestObject& other)
		: x(other.x), y(other.y), z(other.z)
	{
		//Copy
		LOGGER_DEBUG("Copy");
		pNum = new int;
		*pNum = *other.pNum;
	}

	TestObject(TestObject&& other) noexcept
		: x(other.x), y(other.y), z(other.z)
	{
		LOGGER_DEBUG("Move");
		pNum = other.pNum;
		other.pNum = nullptr;
	}

	~TestObject() 
	{
		// Destructor
		LOGGER_DEBUG("Destroy");
		delete pNum;
	}

	TestObject& operator=(const TestObject& other) noexcept
	{
		LOGGER_DEBUG("Copy Assignment");
		x = other.x;
		y = other.y;
		z = other.z;
		pNum = other.pNum;
		return *this;
	}

	TestObject& operator=(TestObject&& other) noexcept
	{
		LOGGER_DEBUG("Move");
		x = std::move(other.x);
		y = std::move(other.y);
		z = std::move(other.z);
		pNum = other.pNum;
		other.pNum = nullptr;
		return *this;
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

void RegisterVectorTests(TestManager& manager)
{
	manager.registerTest(VectorCreateAndDestroy, "vector create and destroy");
	manager.registerTest(VectorIndex, "vector c array like indexing");
	manager.registerTest(VectorObjectConstruct, "vector using object constructor and destructor");
	manager.registerTest(VectorReserve, "vector reserve");
	manager.registerTest(VectorPopBack, "vector pop back");
	manager.registerTest(VectorShrinkToFit, "vector shrink to fit");
	manager.registerTest(VectorResize, "vector resize");
}