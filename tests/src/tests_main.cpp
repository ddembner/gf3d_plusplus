#include "test_macros.hpp"
#include "test_manager.hpp"
#include "test_linear_allocator.hpp"

int main()
{
	Logger::init("tests.log");
	TestManager testManager;
	
	RegisterLinearTests(testManager);

	testManager.runTests();
	return 0;
}