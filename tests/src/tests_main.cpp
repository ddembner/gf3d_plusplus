#include "test_macros.hpp"
#include "test_manager.hpp"
#include "test_linear_allocator.hpp"
#include "test_vector.hpp"

int main()
{
	Logger::init("tests.log");
	TestManager testManager;
	
	RegisterLinearTests(testManager);
	RegisterVectorTests(testManager);

	testManager.runTests();

	Logger::shutdown();
	
	return 0;
}