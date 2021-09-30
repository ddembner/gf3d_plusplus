#include "test_forward_list.hpp"
#include "test_macros.hpp"
#include <containers/forward_list.hpp>
#include <forward_list> //Test against std::forward_list
#include "test_object.hpp"

u8 ForwardCreateAndDestroy()
{
	gf3d::forward_list<int> gf3dNumDefault;

	expect_to_be_true(gf3dNumDefault.begin() == gf3dNumDefault.end());

	gf3d::forward_list<int> gf3dNumSize(1);

	expect_to_be_false(gf3dNumSize.begin() == gf3dNumSize.end());

	return TEST_PASS;
}

u8 ForwardValueCreate()
{
	gf3d::forward_list<int> gf3dNumValue(2, 3);
	u64 count = 0;

	for (auto it = gf3dNumValue.begin(); it != gf3dNumValue.end(); ++it) {
		should_be_equal(3, *it);
		count++;
	}

	should_be_equal(2, count);

	return TEST_PASS;
}

void RegisterForwardListTests(TestManager& manager)
{
	manager.registerTest(ForwardCreateAndDestroy, "Create and destroy forward list");
	manager.registerTest(ForwardValueCreate, "Create using object initialization");
}