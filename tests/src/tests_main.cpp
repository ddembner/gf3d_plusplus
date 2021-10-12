#include "test_macros.hpp"
#include "test_manager.hpp"
#include "test_linear_allocator.hpp"
#include "test_vector.hpp"
#include "test_forward_list.hpp"

#include <glm/glm.hpp>
#include "math/vec3.hpp"

int main()
{
	Logger::init("tests.log");
	TestManager testManager;
	
	RegisterLinearTests(testManager);
	RegisterVectorTests(testManager);
	RegisterForwardListTests(testManager);

	testManager.runTests();

	Logger::shutdown();

	glm::vec3 a = glm::vec3(.25f, 3.2f, 1.7f);
	glm::vec3 b = glm::vec3(.33f, 2.f, .1f);
	glm::vec3 c = glm::cross(a, b);
	gf3d::vec3 x = gf3d::vec3(.25f, 3.2f, 1.7f);
	gf3d::vec3 y = gf3d::vec3(.33f, 2.f, .1f);
	gf3d::vec3 z = x.cross(y);

	GFASSERT(gf3d::fcmp(c.x, z.x), "x didn't match");
	GFASSERT(gf3d::fcmp(c.y, z.y), "y didn't match");
	GFASSERT(gf3d::fcmp(c.z, z.z), "z didn't match");
	
	return 0;
}