#include "test_macros.hpp"
#include "test_manager.hpp"
#include "test_linear_allocator.hpp"
#include "test_vector.hpp"
#include "test_forward_list.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "math/mat4.hpp"

int main()
{
	Logger::init("tests.log");
	TestManager testManager;
	
	RegisterLinearTests(testManager);
	RegisterVectorTests(testManager);
	RegisterForwardListTests(testManager);

	testManager.runTests();

	Logger::shutdown();

	glm::mat4 a = glm::eulerAngleYXZ(45.f, 15.f, 2.f);
	gf3d::mat4 b = gf3d::mat4::eulerAngleYXZ(45.f, 15.f, 2.f);
	
	return 0;
}