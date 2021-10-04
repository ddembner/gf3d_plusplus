#pragma once

#include <defines.hpp>

#define LOG_CONSTRUCTOR_DESTRUCTOR 0

#if LOG_CONSTRUCTOR_DESTRUCTOR
#define LOG_TEST_OBJECT_FUNCTION(msg)	LOGGER_DEBUG(msg)
#else
#define LOG_TEST_OBJECT_FUNCTION(msg) (void(0))		
#endif // LOG_CONSTRUCTOR_DESTRUCTOR


struct TestObject
{
	u32 x, y, z;
	int* pNum = nullptr;

	TestObject() : x(0), y(0), z(0)
	{
		pNum = new int;
		// Default
		LOG_TEST_OBJECT_FUNCTION("Default");
	}

	TestObject(u32 scalar) : x(scalar), y(scalar), z(scalar)
	{
		pNum = new int;
		LOG_TEST_OBJECT_FUNCTION("Scalar");
	}

	TestObject(u32 _x, u32 _y, u32 _z) : x(_x), y(_y), z(_z)
	{
		pNum = new int;
		LOG_TEST_OBJECT_FUNCTION("Construct");
	}


	TestObject(const TestObject& other)
		: x(other.x), y(other.y), z(other.z)
	{
		//Copy
		LOG_TEST_OBJECT_FUNCTION("Copy");
		pNum = new int;
		*pNum = *other.pNum;
	}

	TestObject(TestObject&& other) noexcept
		: x(other.x), y(other.y), z(other.z)
	{
		LOG_TEST_OBJECT_FUNCTION("Move");
		pNum = other.pNum;
		other.pNum = nullptr;
	}

	~TestObject()
	{
		// Destructor
		LOG_TEST_OBJECT_FUNCTION("Destroy");
		delete pNum;
	}

	TestObject& operator=(const TestObject& other) noexcept
	{
		LOG_TEST_OBJECT_FUNCTION("Copy Assignment");
		x = other.x;
		y = other.y;
		z = other.z;
		*pNum = *other.pNum;
		return *this;
	}

	TestObject& operator=(TestObject&& other) noexcept
	{
		LOG_TEST_OBJECT_FUNCTION("Move");
		x = static_cast<int&&>(other.x);
		y = static_cast<int&&>(other.y);
		z = static_cast<int&&>(other.z);
		pNum = other.pNum;
		other.pNum = nullptr;
		return *this;
	}
};