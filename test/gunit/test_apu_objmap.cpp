#include "gtest/gtest.h"
#include "autil_obj.hpp"
#include "autil_objmap.hpp"

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
	int value;
};

class APUObjMapTestFixture : public ::testing::Test
{
protected:
	APUPtr<APUObjectMap<std::string, TestObj> > objMap;

	void SetUp() {
		objMap = new APUObjectMap<std::string, TestObj>();
	}
};

TEST_F(APUObjMapTestFixture, test_put_get)
{
	APUPtr<TestObj> one = new TestObj();
	one->value = 1;
	EXPECT_EQ(1, one->refCount);

	objMap->put("one", one);
	EXPECT_EQ(2, one->refCount);

	APUPtr<TestObj> got = objMap->get("one");
	ASSERT_TRUE((bool)got);
	EXPECT_EQ(1, got->value);
	EXPECT_EQ(3, one->refCount);
}

TEST_F(APUObjMapTestFixture, test_size)
{
	objMap->put("one", new TestObj());
	EXPECT_EQ(1, objMap->size());

	objMap->put("two", new TestObj());
	EXPECT_EQ(2, objMap->size());

	objMap->put("one", NULL);
	EXPECT_EQ(1, objMap->size());
}

TEST_F(APUObjMapTestFixture, test_remove)
{
	TestObj *obj = new TestObj();
	bool didDestroy = false;
	obj->pDidDestroy = &didDestroy;

	objMap->put("one", obj);
	EXPECT_EQ(1, obj->refCount);
	EXPECT_EQ(1, objMap->size());

	objMap->put("one", NULL);
	EXPECT_EQ(0, objMap->size());
	EXPECT_TRUE(didDestroy);
}

TEST_F(APUObjMapTestFixture, test_iterator)
{
	APUPtr<TestObj> one = new TestObj();
	APUPtr<TestObj> two = new TestObj();
	APUPtr<TestObj> three = new TestObj();

	objMap->put("one", one);
	objMap->put("two", two);
	objMap->put("three", three);

	APUObjectMap<std::string, TestObj>::Iterator it = objMap->begin();
	EXPECT_TRUE(it.valid());
	while (it.valid()) {
		TestObj *second = it.second();
		EXPECT_TRUE((second != NULL));
		if (second) {
			second->value = 1;
		}
		++it;
	}

	EXPECT_EQ(1, one->value);
	EXPECT_EQ(1, two->value);
	EXPECT_EQ(1, three->value);
}

TEST_F(APUObjMapTestFixture, test_int_key)
{
	APUPtr<APUObjectMap<int, TestObj> > intKeyMap = new APUObjectMap<int, TestObj>();
	APUPtr<TestObj> one = new TestObj();
	one->value = 1;
	intKeyMap->put(1, one);

	EXPECT_EQ(1, intKeyMap->size());

	APUPtr<TestObj> got = intKeyMap->get(1);
	EXPECT_TRUE((bool)got);
	EXPECT_EQ(1, got->value);
}
