//
//  apu_number.cpp
//  libaputils
//
//  Created by Luke on 8/25/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "apu_number.hpp"
#include <cmath>

APUNumber::APUNumber()
{
    setFloatValue(0);
}

APUNumber::APUNumber(float f)
{
    setFloatValue(f);
}

APUNumberType APUNumber::getType() const
{
    return m_numberType;
}

void APUNumber::setIntegerValue(int32_t i)
{
    m_data.intValue = i;
    m_numberType = APUNumberType::APUNUM_INT;
}

void APUNumber::setUnsignedIntValue(uint32_t u)
{
    m_data.unsignedIntValue = u;
    m_numberType = APUNumberType::APUNUM_UINT;
}

void APUNumber::setFloatValue(float f)
{
    m_data.floatValue = f;
    m_numberType = APUNumberType::APUNUM_FLOAT;
}

void APUNumber::setBoolValue(bool b)
{
    m_data.boolValue = b;
    m_numberType = APUNumberType::APUNUM_BOOLEAN;
}

int32_t APUNumber::integerValue() const
{
    switch (m_numberType) {
        case APUNumberType::APUNUM_INT:
            return m_data.intValue;
        case APUNumberType::APUNUM_UINT:
            return m_data.unsignedIntValue;
        case APUNumberType::APUNUM_FLOAT:
            return (int)floorf(m_data.floatValue);
        case APUNumberType::APUNUM_BOOLEAN:
            return m_data.boolValue ? 1 : 0;
        default:
            return 0;
    }
}

uint32_t APUNumber::unsignedIntValue() const
{
    switch(m_numberType) {
        case APUNumberType::APUNUM_UINT:
            return m_data.unsignedIntValue;
        default:
            return integerValue();
    }
}

float APUNumber::floatValue() const
{
    switch (m_numberType) {
        case APUNumberType::APUNUM_INT:
            return (float)(m_data.intValue);
        case APUNumberType::APUNUM_UINT:
            return (float)(m_data.unsignedIntValue);
        case APUNumberType::APUNUM_FLOAT:
            return m_data.floatValue;
        case APUNumberType::APUNUM_BOOLEAN:
            return m_data.boolValue ? 1.f : 0.f;
        default:
            return 0;
    }
}

bool APUNumber::boolValue() const
{
    int i = integerValue();
    return (i != 0);
}

APUNumber APUNumber::numberForInteger(int i)
{
    APUNumber num;
    num.setIntegerValue(i);

    return num;
}

APUNumber APUNumber::numberForUnsignedInt(uint32_t u)
{
    APUNumber num;
    num.setUnsignedIntValue(u);

    return num;
}

APUNumber APUNumber::numberForFloat(float f)
{
    APUNumber num;
    num.setFloatValue(f);

    return num;
}

APUNumber APUNumber::numberForBool(bool b)
{
    APUNumber num;
    num.setBoolValue(b);

    return num;
}
