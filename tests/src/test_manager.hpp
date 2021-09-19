#pragma once
#include <vector>
#include "defines.hpp"

constexpr u8 TEST_FAIL = static_cast<u8>(0);
constexpr u8 TEST_PASS = static_cast<u8>(1);
constexpr u8 TEST_SKIP = static_cast<u8>(2);

typedef u8 (*PFN_test)();

class TestManager
{
public:
	void registerTest(PFN_test test, const char* description);
	void runTests();
private:

	struct TestEntry
	{
		PFN_test func;
		const char* description;
	};

	std::vector<TestEntry> tests;
};