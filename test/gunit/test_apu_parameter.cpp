#include "gtest/gtest.h"
#include "autil_parameter.hpp"
#include <string.h>

class APUParameterTestFixture : public ::testing::Test
{
protected:
	void SetUp() {

	}

	void TearDown() {

	}
};

TEST_F(APUParameterTestFixture, test_essentials)
{
	APUPtr<APUString> paramName = APUStringMake("param");
	APUParameter param(paramName, APUNUM_FLOAT(0), APUNUM_FLOAT(10), APUNUM_FLOAT(0));
	EXPECT_TRUE(param.getName()->equals(paramName));
	param.setName(APUStringMake("new_name"));
	EXPECT_TRUE(param.getName()->equals("new_name"));

	EXPECT_EQ(APUNUM_FLOAT, param.type());
	EXPECT_EQ(0.0, param.getCurrentValue().floatValue());
	EXPECT_EQ(0.0, param.getTarget().floatValue());

	APUNumber val = APUNUM_FLOAT(7.5);
	param.setValue(val);
	EXPECT_EQ(7.5, param.getCurrentValue().floatValue());
	EXPECT_EQ(7.5, param.getTarget().floatValue());
}

TEST_F(APUParameterTestFixture, test_value_clamping)
{
	APUPtr<APUParameter> param = new APUParameter(APUStringMake("param"), APUNUM_FLOAT(-5), APUNUM_FLOAT(5), APUNUM_FLOAT(0));

	APUNumber val = APUNUM_FLOAT(3.5);
	param->setValue(val);
	EXPECT_EQ(3.5, param->getCurrentValue().floatValue());

	val.setFloatValue(-6.5);
	param->setValue(val);
	EXPECT_EQ(-5.0, param->getCurrentValue().floatValue());

	val.setFloatValue(7.5);
	param->setValue(val);
	EXPECT_EQ(5.0, param->getCurrentValue().floatValue());
}

TEST_F(APUParameterTestFixture, test_units)
{
	APUPtr<APUParameter> param = new APUParameter(APUStringMake("param"), -5, 5, 0);
	param->setUnits(APUStringMake("Hertz"));
	EXPECT_TRUE(param->getUnits()->equals("Hertz"));
}

TEST_F(APUParameterTestFixture, test_type)
{
	APUPtr<APUParameter> param = new APUParameter(APUStringMake("param"), 0, 0, 0);
	EXPECT_EQ(APUNUM_FLOAT, param->type());

	param = new APUParameter(APUStringMake("param"), APUNUM_INT(0), APUNUM_INT(10), APUNUM_INT(0));
	EXPECT_EQ(APUNUM_INT, param->type());
	param->setValue(3.4);
	EXPECT_EQ(APUNUM_FLOAT, param->type());

	param = new APUParameter(APUStringMake("param"), APUNUM_UINT(0), APUNUM_UINT(0), APUNUM_UINT(0));
	EXPECT_EQ(APUNUM_UINT, param->type());
}

TEST_F(APUParameterTestFixture, test_enum_param)
{
	APUPtr<APUEnumerator<APUString> > stringsEnum = new APUEnumerator<APUString>();
	stringsEnum->addObject(APUStringMake("OFF"));
	stringsEnum->addObject(APUStringMake("ON"));

	APUPtr<APUArray<APUString> > strings = new APUArray<APUString>(stringsEnum);
	APUEnumParameter enumParam(APUStringMake("enumParam"), strings.ptr(), NULL);

	APUPtr<APUString> value0Str = enumParam.stringForValue(0);
	EXPECT_TRUE(value0Str->equals("OFF"));

	APUPtr<APUString> value1Str = enumParam.stringForValue(1);
	EXPECT_TRUE(value1Str->equals("ON"));

	EXPECT_EQ(NULL, enumParam.stringForValue(2));

	EXPECT_EQ(APUNUM_UINT, enumParam.type());
}
