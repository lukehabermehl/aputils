#include "gtest/gtest.h"
#include "autil_obj.hpp"
#include "autil_obj_enum.hpp"

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

class APUObjEnumTestFixture : public ::testing::Test
{
protected:
	APUPtr<APUEnumerator<TestObj> > objEnum;

	void SetUp() {
		objEnum = new APUEnumerator<TestObj>();
	}
};

TEST_F(APUObjEnumTestFixture, test_memory)
{
	APUPtr<TestObj> obj1 = new TestObj();
	obj1->value = 1;
	EXPECT_EQ(1, obj1->refCount);

	objEnum->addObject(obj1);
	EXPECT_EQ(2, obj1->refCount);
}

TEST_F(APUObjEnumTestFixture, test_add)
{
	APUPtr<TestObj> obj1 = new TestObj();
	obj1->value = 1;
	APUPtr<TestObj> obj2 = new TestObj();
	obj2->value = 2;

	EXPECT_EQ(NULL, objEnum->getCurrent());
	objEnum->addObject(obj1);
	EXPECT_EQ(obj1.ptr(), objEnum->getCurrent());
	EXPECT_EQ(1, objEnum->size());
	objEnum->addObject(obj2);
	EXPECT_EQ(obj1.ptr(), objEnum->getCurrent());
	EXPECT_EQ(2, objEnum->size());

	objEnum->reset();
	int count = 0;
	do {
		APUPtr<TestObj> current = objEnum->getCurrent();
		EXPECT_TRUE((bool) current);
		EXPECT_EQ(3, current->refCount);
		if (count == 0) {
			EXPECT_EQ(1, current->value);
		} else {
			EXPECT_EQ(2, current->value);
		}

		count++;
	} while (objEnum->moveNext());

	EXPECT_EQ(2, count);
}

TEST_F(APUObjEnumTestFixture, test_remove)
{
	APUPtr<TestObj> obj1 = new TestObj();
	APUPtr<TestObj> obj2 = new TestObj();

	objEnum->addObject(obj1);
	EXPECT_EQ(1, objEnum->size());
	objEnum->removeObject(obj1);
	EXPECT_EQ(0, objEnum->size());
	EXPECT_EQ(1, obj1->refCount);

	objEnum->addObject(obj1);
	objEnum->addObject(obj2);
	EXPECT_EQ(2, objEnum->size());
	objEnum->removeObject(obj2);
	EXPECT_EQ(1, objEnum->size());
	EXPECT_EQ(1, obj2->refCount);
}
