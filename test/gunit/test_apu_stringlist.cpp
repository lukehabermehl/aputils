#include "gtest/gtest.h"
#include "autil_stringlist.hpp"

class APUStringListTestFixture : public ::testing::Test
{
protected:
	APUPtr<APUStringList> stringList;

	void SetUp() {
		stringList = new APUStringList();
	}
};

TEST_F(APUStringListTestFixture, test_empty_list)
{
	EXPECT_EQ(0, stringList->size());
	EXPECT_EQ(NULL, stringList->getString(0));
}

TEST_F(APUStringListTestFixture, test_append)
{
	stringList->append("one");
	EXPECT_EQ(1, stringList->size());
	EXPECT_EQ("one", std::string(stringList->getString(0)));
	EXPECT_EQ(NULL, stringList->getString(1));

	stringList->append("two");
	EXPECT_EQ(2, stringList->size());
	EXPECT_EQ("two", std::string(stringList->getString(1)));
}

TEST_F(APUStringListTestFixture, test_set_string)
{
	stringList->append("one");
	EXPECT_EQ(1, stringList->size());
	stringList->setString(0, "two");
	EXPECT_EQ(1, stringList->size());
	EXPECT_EQ("two", std::string(stringList->getString(0)));
}

TEST_F(APUStringListTestFixture, test_copy)
{
	stringList->append("one");
	stringList->append("two");

	APUPtr<APUStringList> listCopy = stringList->copy();
	EXPECT_EQ(2, listCopy->size());
	EXPECT_EQ("one", std::string(listCopy->getString(0)));
	EXPECT_EQ("two", std::string(listCopy->getString(1)));

	stringList->append("three");
	EXPECT_EQ(3, stringList->size());
	EXPECT_EQ(2, listCopy->size());
}