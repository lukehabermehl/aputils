#include "gtest/gtest.h"
#include "apu_str.hpp"

class APUStringTestFixture : public ::testing::Test
{
};

TEST_F(APUStringTestFixture, test_basics)
{
	APUPtr<APUString> str = APUStringMake("Hello, world!");
	EXPECT_EQ(13, str->length());

	EXPECT_EQ(0, strcmp("Hello, world!", str->str()));
}

TEST_F(APUStringTestFixture, test_compare)
{
	APUPtr<APUString> str1 = APUStringMake("one");
	APUPtr<APUString> str2 = APUStringMake("two");

	EXPECT_TRUE(str1->compare(str2));
	EXPECT_FALSE(str2->compare(str1));

	str2 = (APUString *)APUStringMake("one");

	EXPECT_FALSE(str1->compare(str2));
	EXPECT_FALSE(str2->compare(str1));

	EXPECT_TRUE(str1->equals(str2));
}

TEST_F(APUStringTestFixture, test_append)
{
	APUPtr<APUString> string1 = APUStringMake("Hello");
	APUPtr<APUString> string2 = APUStringMake(", world!");
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

TEST_F(APUStringTestFixture, test_stringref)
{
	APUPtr<APUString> pStr = APUStringMake("Hello");
	APUStringRef sref(pStr);

	// Test implicit conversion to const char *
	EXPECT_EQ(0, strcmp(sref, "Hello"));

	EXPECT_EQ(5, sref.length());
	EXPECT_FALSE(sref.isEmpty());

	// Let's try something fancy
	sref.append(", world!");
	EXPECT_TRUE(sref == "Hello, world!");

	APUStringRef strref = APUStringMake("onetwothree").ptr();
	EXPECT_TRUE(strref == "onetwothree");
	pStr = strref.get();
	EXPECT_TRUE(pStr->equals("onetwothree"));
	EXPECT_EQ(2, pStr->getRefCount());

	pStr = APUStringRef("fourFiveSix");
	EXPECT_TRUE(pStr->equals("fourFiveSix"));
	EXPECT_EQ(1, pStr->getRefCount());
}
