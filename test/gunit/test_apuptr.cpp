#include "gtest/gtest.h"
#include "autil_obj.hpp"

class TestObj : public APUObject
{
public:
	APUOBJ_FWDDECL

	TestObj() : APUObject() {}
	~TestObj() {
		*pDidDestroy = true;
	}

	bool *pDidDestroy;
};

class TestContainer
{
public:
	APUPtr<TestObj> obj;
	TestContainer() {
		obj = new TestObj();
	}
};

class APUPtrTestFixture : public ::testing::Test
{
};

TEST_F(APUPtrTestFixture, test_delete)
{
	TestContainer *container = new TestContainer();
	bool didDestroy = false;
	container->obj->pDidDestroy = &didDestroy;
	EXPECT_FALSE(didDestroy);

	delete container;
	EXPECT_TRUE(didDestroy);
}

TEST_F(APUPtrTestFixture, test_retain)
{
	TestContainer *container = new TestContainer();
	bool didDestroy = false;
	container->obj->pDidDestroy = &didDestroy;
	EXPECT_FALSE(didDestroy);

	APUPtr<TestObj> retainObj = container->obj;
	delete container;
	EXPECT_FALSE(didDestroy);
}

TEST_F(APUPtrTestFixture, test_clear)
{
	APUPtr<TestObj> obj(new TestObj());
	bool didDestroy = false;
	obj->pDidDestroy = &didDestroy;
	obj.clear();

	EXPECT_TRUE(didDestroy);
	EXPECT_EQ(NULL, obj.ptr());
}