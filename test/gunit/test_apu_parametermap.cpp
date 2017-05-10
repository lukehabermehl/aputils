#include "gtest/gtest.h"
#include "autil_parametermap.hpp"

class APUParameterMapTestFixture : public ::testing::Test
{
protected:
	APUPtr<APUParameterMap> paramMap;
	void SetUp()
	{
		paramMap = new APUParameterMap();
		paramMap->addParameter(new APUParameter("param1", APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(100), APUNUM_FLOAT(0)));
		paramMap->addParameter(new APUParameter("param2", APUNUM_BOOLEAN, APUNUM_BOOL(0), APUNUM_BOOL(1), APUNUM_BOOL(0)));
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
		std::string name = it.first();
		if (name == "param1")
		{
			found[0] = true;
			EXPECT_TRUE(it.second() != NULL);
		}
		else if (name == "param2")
		{
			found[1] = true;
			EXPECT_TRUE(it.second()!= NULL);
		}
		else
		{
			std::cout << "name was: " << name << std::endl;
			EXPECT_TRUE(false); //Something bad happened
		}
	}

	EXPECT_TRUE(found[0]);
	EXPECT_TRUE(found[1]);
}

TEST_F(APUParameterMapTestFixture, test_api)
{
	APUParameter * param1 = paramMap->parameterWithName("param1");
	EXPECT_TRUE(param1 != NULL);
	EXPECT_EQ(APUNUM_FLOAT, param1->type());

	APUParameter * param2 = paramMap->parameterWithName("param2");
	EXPECT_TRUE(param2 != NULL);
	EXPECT_EQ(APUNUM_BOOLEAN, param2->type());

	EXPECT_EQ(NULL, paramMap->parameterWithName("notaparam"));
}

TEST_F(APUParameterMapTestFixture, test_memory)
{
	APUParameterMap *memParamMap = new APUParameterMap();
	APUPtr<APUParameter> param = new APUParameter("memParam", APUNUM_FLOAT, APUNUM_FLOAT(0), APUNUM_FLOAT(100), APUNUM_FLOAT(0));
	memParamMap->addParameter(param);
	EXPECT_EQ(3, param->addRef());
	EXPECT_EQ(2, param->decRef());

	delete memParamMap;
	EXPECT_EQ(2, param->addRef());
	EXPECT_EQ(1, param->decRef());
}

