#include "gtest/gtest.h"
#include "autil_array.hpp"
#include "autil_obj_enum.hpp"
#include "autil_str.hpp"

#include <string>

class APUArrayTestFixture : public ::testing::Test
{
protected:
	typedef APUArray<APUString> APUStringArray;
	void SetUp() {

	}

	void TearDown() {

	}

	void initEnum() {
		m_enum = new APUEnumerator<APUString>();
		m_enum->addObject(new APUString("one"));
		m_enum->addObject(new APUString("two"));
		m_enum->addObject(new APUString("three"));
	}

	APUPtr<APUEnumerator<APUString> > m_enum;
};

TEST_F(APUArrayTestFixture, test_empty)
{
	APUPtr<APUStringArray> arr = new APUStringArray();
	EXPECT_EQ(0, arr->size());
	EXPECT_FALSE((bool) arr->getAt(0));
	EXPECT_FALSE((bool) arr->getCurrent());
}

TEST_F(APUArrayTestFixture, test_getat)
{
	initEnum();
	ASSERT_EQ(3, m_enum->size());

	APUPtr<APUStringArray> arr = new APUStringArray(m_enum);
	EXPECT_EQ(3, arr->size());

	APUString *str = arr->getAt(0);
	ASSERT_TRUE((str != NULL));
	EXPECT_EQ(0, strcmp(str->str(), "one"));

	str = arr->getAt(1);
	ASSERT_TRUE((str != NULL));
	EXPECT_EQ(0, strcmp(str->str(), "two"));

	str = arr->getAt(2);
	ASSERT_TRUE((str != NULL));
	EXPECT_EQ(0, strcmp(str->str(), "three"));

	EXPECT_FALSE((bool)arr->getAt(3));
}

TEST_F(APUArrayTestFixture, test_enum)
{
	initEnum();
	ASSERT_EQ(3, m_enum->size());

	APUPtr<APUStringArray> arr = new APUStringArray(m_enum);
	ASSERT_EQ(3, arr->size());

	arr->reset();
	ASSERT_TRUE((bool)arr->getCurrent());

	for (int i=0; i<3; i++) {
		APUPtr<APUString> current = arr->getCurrent();
		APUPtr<APUString> cmp = arr->getAt(i);
		EXPECT_EQ(current.ptr(), cmp.ptr());
		if (i != 2) {
			EXPECT_TRUE(arr->moveNext());
		}
	}

	EXPECT_FALSE(arr->moveNext());
	EXPECT_FALSE((bool)arr->getCurrent());
}
