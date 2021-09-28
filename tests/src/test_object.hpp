#pragma once

#include <defines.hpp>

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
		// LOGGER_DEBUG("Copy");
		pNum = new int;
		*pNum = *other.pNum;
	}

	TestObject(TestObject&& other) noexcept
		: x(other.x), y(other.y), z(other.z)
	{
		// LOGGER_DEBUG("Move");
		pNum = other.pNum;
		other.pNum = nullptr;
	}

	~TestObject()
	{
		// Destructor
		// LOGGER_DEBUG("Destroy");
		delete pNum;
	}

	TestObject& operator=(const TestObject& other) noexcept
	{
		// LOGGER_DEBUG("Copy Assignment");
		x = other.x;
		y = other.y;
		z = other.z;
		pNum = other.pNum;
		return *this;
	}

	TestObject& operator=(TestObject&& other) noexcept
	{
		// LOGGER_DEBUG("Move");
		x = static_cast<int&&>(other.x);
		y = static_cast<int&&>(other.y);
		z = static_cast<int&&>(other.z);
		pNum = other.pNum;
		other.pNum = nullptr;
		return *this;
	}
};