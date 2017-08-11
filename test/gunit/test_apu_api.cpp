#include "gtest/gtest.h"
#include "autil_parameter.hpp"
#include "autil_parametermap.hpp"
#include "autil_audioprocessingunit.hpp"

class APUApiTestAPU : public AudioProcessingUnit
{
public:
	APUApiTestAPU() : AudioProcessingUnit()
	{
		APUParameter *param1 = new APUParameter(APUStringMake("param1"), APUNUM_FLOAT(0), APUNUM_FLOAT(100), APUNUM_FLOAT(0));
		APUParameter *param2 = new APUParameter(APUStringMake("param2"), APUNUM_BOOL(0), APUNUM_BOOL(1), APUNUM_BOOL(0));

		addParameter(param1);
		addParameter(param2);
	}
};

class APUApiTestFixture : public ::testing::Test
{
protected:
	APUApiTestAPU *apu;

	void SetUp()
	{
		apu = new APUApiTestAPU();
	}

	void TearDown()
	{
		delete apu;
	}
};

TEST_F(APUApiTestFixture, test_param_lookup)
{
	APUPtr<APUParameter> param1 = apu->getParameterWithName("param1");
	EXPECT_TRUE(param1);
	EXPECT_EQ(APUNUM_FLOAT, param1->type());

	APUPtr<APUParameter> param2 = apu->getParameterWithName("param2");
	EXPECT_TRUE(param2);
	EXPECT_EQ(APUNUM_BOOLEAN, param2->type());

	APUPtr<APUParameter> nullParam = apu->getParameterWithName("shouldNotExist");
	EXPECT_FALSE(nullParam);
}

TEST_F(APUApiTestFixture, test_param_list)
{
	const APUParameterMap * params = apu->getParameterMap();

	EXPECT_EQ(2, params->size());
}
