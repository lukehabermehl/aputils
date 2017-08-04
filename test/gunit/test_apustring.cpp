#include "gtest/gtest.h"
#include "autil_str.hpp"

class APUStringTestFixture : public ::testing::Test
{
};

TEST_F(APUStringTestFixture, test_basics)
{
	APUPtr<APUString> str = new APUString("Hello, world!");
	EXPECT_EQ(13, str->length());

	EXPECT_EQ(0, strcmp("Hello, world!", str->str()));
}

TEST_F(APUStringTestFixture, test_compare)
{
	APUPtr<APUString> str1 = new APUString("one");
	APUPtr<APUString> str2 = new APUString("two");

	EXPECT_TRUE(str1->compare(str2));
	EXPECT_FALSE(str2->compare(str1));

	str2 = new APUString("one");

	EXPECT_FALSE(str1->compare(str2));
	EXPECT_FALSE(str2->compare(str1));

	EXPECT_TRUE(str1->equals(str2));
}

TEST_F(APUStringTestFixture, test_append)
{
	APUPtr<APUString> string1 = new APUString("Hello");
	APUPtr<APUString> string2 = new APUString(", world!");
	APUPtr<APUString> combined = string1->append(string2);

	EXPECT_TRUE(combined->equals("Hello, world!"));
}

TEST_F(APUStringTestFixture, test_format)
{
	APUPtr<APUString> formatted = APUString::format("%s, %s!", "Hello", "world");
	EXPECT_EQ(0, strcmp(formatted->str(), "Hello, world!"));
}

struct MemoryTestHelper
{
	APUPtr<APUString> str;
	MemoryTestHelper(APUString *paramStr)
	: str(paramStr)
	{
	}
};

TEST_F(APUStringTestFixture, test_memory)
{
	MemoryTestHelper helper(APUStringMake("test"));
	EXPECT_TRUE(helper.str);
	EXPECT_EQ(1, helper.str->getRefCount());
}