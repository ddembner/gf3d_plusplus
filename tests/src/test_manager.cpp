#include "test_manager.hpp"
#include "core/gf3d_logger.h"

void TestManager::registerTest(PFN_test test, const char* description)
{
	tests.push_back({ test, description });
}

void TestManager::runTests()
{
	u32 passed = 0, skipped = 0, failed = 0;

	for (auto& test : tests) {
		u8 result = test.func();

		switch (result)
		{
		case TEST_PASS:
			passed++;
			break;
		case TEST_SKIP:
			skipped++;
			LOGGER_WARN("Skipped {0}", test.description);
			break;
		case TEST_FAIL:
			failed++;
			LOGGER_ERROR("Test Failed: {0}", test.description);
			break;
		default:
			LOGGER_ERROR("Test did not return proper value: test: {0}, value: {1}", test.description, result);
		}
	}

	LOGGER_INFO("Results: Passed {0}, Skipped {1}, Failed: {2}", passed, skipped, failed);
}