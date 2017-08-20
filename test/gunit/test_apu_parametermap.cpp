#include "gtest/gtest.h"
#include "apu_parametermap.hpp"

class APUParameterMapTestFixture : public ::testing::Test
{
protected:
	APUPtr<APUParameterMap> paramMap;
	void SetUp()
	{
		paramMap = new APUParameterMap();
		paramMap->addParameter(new APUParameter(APUStringMake("param1"), APUNUM_FLOAT(0), APUNUM_FLOAT(100), APUNUM_FLOAT(0)));
		paramMap->addParameter(new APUParameter(APUStringMake("param2"), APUNUM_BOOL(0), APUNUM_BOOL(1), APUNUM_BOOL(0)));
	}
};

TEST_F(APUParameterMapTestFixture, test_iterator)
{
	ASSERT_EQ(2, paramMap->size());

	APUParameterMap::Iterator it = paramMap->begin();

	EXPECT_TRUE(it.valid());
	bool found[] = {false, false};

	for (; it.valid(); ++it)
	{
		APUPtr<APUString> name = it.first();
		if (name->equals("param1"))
		{
			found[0] = true;
			EXPECT_TRUE(it.second());
		}
		else if (name->equals("param2"))
		{
			found[1] = true;
			EXPECT_TRUE(it.second());
		}
		else
		{
			std::cout << "name was: " << name->str() << std::endl;
			EXPECT_TRUE(false); //Something bad happened
		}
	}

	EXPECT_TRUE(found[0]);
	EXPECT_TRUE(found[1]);
}

TEST_F(APUParameterMapTestFixture, test_api)
{
	APUParameter * param1 = paramMap->parameterWithIdentifier("param1");
	EXPECT_TRUE(param1 != NULL);
	EXPECT_EQ(APUNUM_FLOAT, param1->type());

	APUParameter * param2 = paramMap->parameterWithIdentifier("param2");
	EXPECT_TRUE(param2 != NULL);
	EXPECT_EQ(APUNUM_BOOLEAN, param2->type());

	EXPECT_EQ(NULL, paramMap->parameterWithIdentifier("notaparam"));
}

TEST_F(APUParameterMapTestFixture, test_memory)
{
	APUParameterMap *memParamMap = new APUParameterMap();
	APUPtr<APUParameter> param = new APUParameter(APUStringMake("memParam"), APUNUM_FLOAT(0), APUNUM_FLOAT(100), APUNUM_FLOAT(0));
	memParamMap->addParameter(param);
	EXPECT_EQ(3, param->addRef());
	EXPECT_EQ(2, param->decRef());

	delete memParamMap;
	EXPECT_EQ(2, param->addRef());
	EXPECT_EQ(1, param->decRef());
}

