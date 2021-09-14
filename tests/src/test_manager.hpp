#pragma once
#include <vector>
#include "defines.hpp"

constexpr u8 TEST_FAIL = 0;
constexpr u8 TEST_PASS = 1;
constexpr u8 TEST_SKIP = 2;

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