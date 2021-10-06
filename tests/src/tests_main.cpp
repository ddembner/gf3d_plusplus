#include "test_macros.hpp"
#include "test_manager.hpp"
#include "test_linear_allocator.hpp"
#include "test_vector.hpp"
#include "test_forward_list.hpp"
#include "test_vec2.hpp"

int main()
{
	Logger::init("tests.log");
	TestManager testManager;
	
	RegisterLinearTests(testManager);
	RegisterVectorTests(testManager);
	RegisterForwardListTests(testManager);
	RegisterVec2Tests(testManager);

	testManager.runTests();

	Logger::shutdown();
	
	return 0;
}