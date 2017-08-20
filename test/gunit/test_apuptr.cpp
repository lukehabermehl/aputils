#include "gtest/gtest.h"
#include "apu_obj.hpp"

class TestObj : public APUObject
{
public:
	APUOBJ_FWDDECL
	TestObj() : pDidDestroy(NULL) {}
	TestObj(bool *pDidDestroy) : pDidDestroy(pDidDestroy) {}
	~TestObj() {
		if (pDidDestroy) {
			*pDidDestroy = true;
		}
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

	APUObjRet<TestObj> getObj() { return obj; }
	void setObj(TestObj *ob) { obj = ob; }
};

class APUPtrTestFixture : public ::testing::Test
{
};

APUObjRet<TestObj> get_temp_object(bool *pDidDestroy=NULL)
{
	return new TestObj(pDidDestroy);
}

int get_ref_count(APUObjectInterface *obj)
{
	return obj->getRefCount();
}

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
	EXPECT_EQ(2, obj->getRefCount());

	delete container;

	TestContainer otherContainer;
	EXPECT_EQ(1, get_ref_count(otherContainer.getObj()));

	bool didDestroy = false;
	EXPECT_EQ(0, get_ref_count(get_temp_object(&didDestroy)));
	EXPECT_TRUE(didDestroy);
}

TEST_F(APUPtrTestFixture, test_pass)
{
	TestContainer *container = new TestContainer();
	APUPtr<TestObj> obj = new TestObj();
	EXPECT_EQ(1, obj->getRefCount());
	container->setObj(obj);

	EXPECT_EQ(2, obj->getRefCount());

	delete container;
}

TEST_F(APUPtrTestFixture, test_reassign)
{
	bool firstDidDelete = false;
	APUPtr<TestObj> obj = new TestObj(&firstDidDelete);
	EXPECT_FALSE(firstDidDelete);

	obj = new TestObj();
	EXPECT_TRUE(firstDidDelete);

	APUPtr<TestObj> obj2 = obj;
	EXPECT_EQ(2, obj->getRefCount());

	obj2 = new TestObj();
	EXPECT_EQ(1, obj->getRefCount());
}
