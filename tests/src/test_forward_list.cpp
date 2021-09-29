#include "test_forward_list.hpp"
#include "test_macros.hpp"
#include <containers/forward_list.hpp>
#include <forward_list> //Test against std::forward_list
#include "test_object.hpp"

u8 ForwardCreateAndDestroy()
{
	LOGGER_TRACE("==== forward start ====");
	std::forward_list<TestObject> stdNum;
	gf3d::forward_list<TestObject> gf3dNum;

	// expect_to_be_true(gf3dNum.begin() == gf3dNum.end());

	auto before = stdNum.before_begin();
	auto gf3dBegin = gf3dNum.before_begin();
	LOGGER_DEBUG(before->x);
	LOGGER_DEBUG(gf3dBegin->x);

	// should_be_equal(7, *stdNum.begin());

	return TEST_PASS;
}

void RegisterForwardListTests(TestManager& manager)
{
	manager.registerTest(ForwardCreateAndDestroy, "Create and destroy forward list");
}