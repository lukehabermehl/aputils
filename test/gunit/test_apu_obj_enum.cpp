#include "gtest/gtest.h"
#include "autil_obj.hpp"
#include "autil_obj_enum.hpp"

class TestObj : public APUObject
{
public:
	APUOBJ_FWDDECL
	TestObj() : pDidDestroy(NULL) {}
	~TestObj() {
		if (pDidDestroy) {
			*pDidDestroy = true;
		}
	}

	bool *pDidDestroy;
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
	EXPECT_EQ(1, obj1->getRefCount());

	objEnum->addObject(obj1);
	EXPECT_EQ(2, obj1->getRefCount());

	APUPtr<TestObj> rv = objEnum->getCurrent();
	EXPECT_TRUE((bool)rv);
	EXPECT_EQ(3, rv->getRefCount());

	rv.clear();
	EXPECT_EQ(2, obj1->getRefCount());

	TestObj *weakrv = objEnum->getCurrent();
	ASSERT_TRUE(weakrv);
	EXPECT_EQ(2, weakrv->getRefCount());
}

TEST_F(APUObjEnumTestFixture, test_add)
{
	APUPtr<TestObj> obj1 = new TestObj();
	obj1->value = 1;
	APUPtr<TestObj> obj2 = new TestObj();
	obj2->value = 2;

	EXPECT_FALSE((bool)objEnum->getCurrent());
	objEnum->addObject(obj1);
	EXPECT_EQ(obj1.ptr(), objEnum->getCurrent().ptr());
	EXPECT_EQ(1, objEnum->size());
	objEnum->addObject(obj2);
	EXPECT_EQ(obj1.ptr(), objEnum->getCurrent().ptr());
	EXPECT_EQ(2, objEnum->size());

	objEnum->reset();
	int count = 0;
	do {
		APUPtr<TestObj> current = objEnum->getCurrent();
		EXPECT_TRUE((bool) current);
		EXPECT_EQ(3, current->getRefCount());
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
	EXPECT_EQ(1, obj1->getRefCount());

	objEnum->addObject(obj1);
	objEnum->addObject(obj2);
	EXPECT_EQ(2, objEnum->size());
	objEnum->removeObject(obj2);
	EXPECT_EQ(1, objEnum->size());
	EXPECT_EQ(1, obj2->getRefCount());
}
