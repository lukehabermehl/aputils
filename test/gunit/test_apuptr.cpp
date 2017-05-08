#include "gtest/gtest.h"
#include "autil_obj.hpp"

class TestObj : public APUObjectInterface
{
public:
	TestObj() : pDidDestroy(NULL), refCount(0) {}
	~TestObj() {
		if (pDidDestroy) {
			*pDidDestroy = true;
		}
	}

	int addRef() { 
		return ++refCount;
	}
	int decRef() { 
		return --refCount; 
	}

	bool *pDidDestroy;
	int refCount;
};

class TestContainer
{
public:
	APUPtr<TestObj> obj;
	TestContainer() {
		obj = new TestObj();
	}

	APUPtr<TestObj> getObj() { return obj; }
	void setObj(TestObj *ob) { obj = ob; }
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

TEST_F(APUPtrTestFixture, test_return)
{
	TestContainer *container = new TestContainer();
	APUPtr<TestObj> obj = container->getObj();
	EXPECT_EQ(2, obj->refCount);

	delete container;
}

TEST_F(APUPtrTestFixture, test_pass)
{
	TestContainer *container = new TestContainer();
	APUPtr<TestObj> obj = new TestObj();
	EXPECT_EQ(1, obj->refCount);
	container->setObj(obj);

	EXPECT_EQ(2, obj->refCount);

	delete container;
}
