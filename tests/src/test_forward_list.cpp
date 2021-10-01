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
	const u64 total = 100;
	std::forward_list<int> gf3dNumValue(total, 3);
	u64 count = 0;

	for (auto it = gf3dNumValue.begin(); it != gf3dNumValue.end(); ++it) {
		should_be_equal(3, *it);
		count++;
	}

	should_be_equal(total, count);

	return TEST_PASS;
}

u8 ForwardEmplaceFront()
{
	gf3d::forward_list<int> numList;

	numList.emplace_front(1);

	should_be_equal(1, *numList.begin());

	numList.emplace_front(4);

	should_be_equal(4, *numList.begin());

	int& num = numList.emplace_front(7);

	should_be_equal(7, *numList.begin());

	num = 3;

	should_be_equal(3, *numList.begin());

	return TEST_PASS;
}

u8 ForwardPushFront()
{
	gf3d::forward_list<int> numList;

	int num = 1;

	numList.push_front(num);

	should_be_equal(1, *numList.begin());

	numList.push_front(2);

	should_be_equal(2, *numList.begin());

	return TEST_PASS;
}

u8 ForwardObjectTest()
{
	gf3d::forward_list<TestObject> objects(2, TestObject(1, 2, 3));

	should_be_equal(3, objects.begin()->z);

	objects.emplace_front(TestObject(7));

	should_be_equal(7, objects.begin()->x);

	TestObject obj;

	objects.push_front(obj);

	should_be_equal(0, objects.begin()->y);

	objects.clear();

	expect_to_be_true(objects.is_empty());

	return TEST_PASS;
}

u8 ForwardCopyAssignment()
{
	/*std::forward_list<int> listSource;
	gf3d::forward_list<int> listDestination;

	listSource.emplace_front(3);
	listSource.emplace_front(2);
	listSource.emplace_front(1);

	listDestination = listSource;

	*listDestination.begin() = 4;

	should_be_equal(4, *listDestination.begin());
	should_not_be_equal(4, *listSource.begin());*/

	return TEST_PASS;
}

void RegisterForwardListTests(TestManager& manager)
{
	manager.registerTest(ForwardCreateAndDestroy, "Create and destroy forward list");
	manager.registerTest(ForwardValueCreate, "Create using object initialization in forward list");
	manager.registerTest(ForwardEmplaceFront, "Emplace object to front of forward list");
	manager.registerTest(ForwardPushFront, "Push object to front of forward list");
	manager.registerTest(ForwardObjectTest, "Test for object behavior in forward list");
	manager.registerTest(ForwardCopyAssignment, "Copy assignment for forward list");
}