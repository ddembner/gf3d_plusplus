#include "test_forward_list.hpp"
#include "test_macros.hpp"
#include <containers/forward_list.hpp>
#include <forward_list> //Test against std::forward_list
#include "test_object.hpp"

u8 ForwardCreateAndDestroy()
{
	gf3d::forward_list<int> gf3dNumDefault;

	expect_to_be_true(gf3dNumDefault.begin() == gf3dNumDefault.end());

	LOGGER_DEBUG("==== list start ====");

	std::forward_list<int> stdNumSize(3);
	gf3d::forward_list<int> gf3dNumSize(3);

	expect_to_be_false(gf3dNumSize.begin() == gf3dNumSize.end());

	return TEST_PASS;
}

void RegisterForwardListTests(TestManager& manager)
{
	manager.registerTest(ForwardCreateAndDestroy, "Create and destroy forward list");
}