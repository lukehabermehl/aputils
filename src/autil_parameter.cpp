//
//  autil_parameter.cpp
//  libblockdsp
//
//  Created by Luke on 8/26/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_parameter.hpp"
#include "autil_parameter_private.hpp"
#include "autil_logger.hpp"
#include <cmath>
#include <climits>
#include <assert.h>

#define MIN2(_a, _b) ((_a <= _b) ? _a : _b)

APUParameter::APUParameter(APUString *name, APUNumber minValue, APUNumber maxValue, APUNumber defaultValue, APUParameterCallback *cb)
{
    m_pimpl = new Pimpl(name, cb);
    setMinValue(minValue);
    setMaxValue(maxValue);
    m_pimpl->current = defaultValue;
    m_pimpl->target = defaultValue;
}

APUParameter::~APUParameter()
{
    delete m_pimpl;
}

APUObjRet<APUString> APUParameter::getName()
{
    return m_pimpl->name;
}

void APUParameter::setName(APUString *name)
{
    m_pimpl->name = name;
}

APUObjRet<APUString> APUParameter::getUnits()
{
    return m_pimpl->units;
}

void APUParameter::setUnits(APUString *units)
{
    m_pimpl->units = units;
}

APUNumberType APUParameter::type()
{
    return m_pimpl->target.getType();
}

void APUParameter::setCallback(APUParameterCallback *cb)
{
    m_pimpl->cb = cb;
}

APUNumber APUParameter::getTarget()
{
    return m_pimpl->target;
}

APUNumber APUParameter::getCurrentValue()
{
    m_pimpl->doModulate();
    normalizeValue(m_pimpl->current);
    return m_pimpl->current;
}

bool APUParameter::setValue(APUNumber value)
{
    bool ret = normalizeValue(value);
    m_pimpl->isSmoothing = false;
    m_pimpl->current = value;
    m_pimpl->target = value;
    m_pimpl->base = value;

    if (m_pimpl->cb) {
        m_pimpl->cb->onParameterChanged(this);
    }

    return !ret;
}

bool APUParameter::setTarget(APUNumber target)
{
    bool ret = normalizeValue(target);
    if (!isSmoothingEnabled()) {
        setValue(target);
    } else {
        m_pimpl->isSmoothing = true;
        m_pimpl->target = target;
        m_pimpl->diffPerUpdate = (m_pimpl->target.floatValue() - m_pimpl->current.floatValue()) / (float)m_pimpl->smoothingFrames;
    }

    return !ret;
}

void APUParameter::setMinValue(APUNumber minVal)
{
    m_pimpl->minValue = minVal;
}

void APUParameter::setMaxValue(APUNumber maxVal)
{
    m_pimpl->maxValue = maxVal;
}

APUNumber APUParameter::getMinValue()
{
    APUNumber num;
    num = m_pimpl->minValue;
    return num;
}

APUNumber APUParameter::getMaxValue()
{
    APUNumber num;
    num = m_pimpl->maxValue;
    return num;
}

void APUParameter::setSmoothingEnabled(bool enabled)
{
    if (type() == APUNUM_FLOAT)
        m_pimpl->smoothingEnabled = enabled;
}

void APUParameter::setSmoothingInterval(double millisec)
{
    m_pimpl->smoothingInterval_msec = millisec;
}

double APUParameter::getSmoothingInterval()
{
    return m_pimpl->smoothingInterval_msec;
}

bool APUParameter::isSmoothingEnabled()
{
    return m_pimpl->smoothingEnabled;
}

APUUIAttribute APUParameter::getUIAttributes()
{
    return m_pimpl->uiAttr;
}

void APUParameter::update()
{
    if (m_pimpl->isSmoothing) {
        float cv = m_pimpl->current.floatValue();
        cv += m_pimpl->diffPerUpdate;
        m_pimpl->current.setFloatValue(cv);
        m_pimpl->base.setFloatValue(cv);
        
        if (cv == m_pimpl->target.floatValue()) {
            m_pimpl->isSmoothing = false;
        }

        if (m_pimpl->cb) {
            m_pimpl->cb->onParameterChanged(this);
        }
    }
}

void APUParameter::setSampleRate(size_t sampleRate)
{
    double dSampleRate = (double)sampleRate;
    double dFramesPerUpdate = dSampleRate * m_pimpl->smoothingInterval_msec * 1000;
    m_pimpl->smoothingFrames = floor(dFramesPerUpdate);
}

bool APUParameter::normalizeValue(APUNumber &value)
{
    bool didNormalize = false;
    switch (type())
    {
        case APUNUM_FLOAT:
        {
            if (value.floatValue() < m_pimpl->minValue.floatValue()) {
                value.setFloatValue(m_pimpl->minValue.floatValue());
                didNormalize = true;
            } else if (value.floatValue() > m_pimpl->maxValue.floatValue()) {
                value.setFloatValue(m_pimpl->maxValue.floatValue());
                didNormalize = true;
            }
            break;
        }
        case APUNUM_INT:
        {
            if (value.integerValue() < m_pimpl->minValue.integerValue()) {
                value.setIntegerValue(m_pimpl->minValue.integerValue());
                didNormalize = true;
            } else if (value.integerValue() > m_pimpl->maxValue.integerValue()) {
                value.setIntegerValue(m_pimpl->maxValue.integerValue());
                didNormalize = true;
            }
            break;
        }
        default:
            break;
    }

    return didNormalize;
}

void APUParameter::setModulationDepth(float depth)
{
    m_pimpl->modRange = depth * (m_pimpl->maxValue.floatValue() - m_pimpl->minValue.floatValue());
}

void APUParameter::setModulationSource(APUModSource *source)
{
    m_pimpl->modSource = source;
}

void APUParameter::setUIAttributes(APUUIAttribute attr)
{
    m_pimpl->uiAttr = attr;
}

//----------------------------------------------------------------------------
// APUEnumParameter
//----------------------------------------------------------------------------

APUEnumParameter::APUEnumParameter(APUString *name, APUArray<APUString> *strings, APUParameterCallback *cb)
: APUParameter(name, APUNUM_UINT(0), APUNUM_UINT((uint32_t)strings->size()), APUNUM_UINT(0), cb)
{
    m_enumParamPimpl = new EnumParamPimpl();
    if (strings) {
        m_enumParamPimpl->strings = strings;
    }
    else {
        m_enumParamPimpl->strings = NULL;
    }
}

APUEnumParameter::~APUEnumParameter()
{
    delete m_enumParamPimpl;
}

APUObjRet<APUString> APUEnumParameter::stringForValue(uint32_t value)
{
    if (!m_enumParamPimpl->strings) {
        return NULL;
    }

    return m_enumParamPimpl->strings->getAt(value);
}

void APUEnumParameter::setMinValue(APUNumber minVal)
{
    //No-op
}

void APUEnumParameter::setMaxValue(APUNumber maxVal)
{
    //No-op
}

//---------------------------------------------------------
// APUParameter::Pimpl
//---------------------------------------------------------

void
APUParameter::Pimpl::doModulate()
{
    if (!modSource || !modRange) {
        return;
    }

    float fCurrent = base.floatValue();
    float fModValue = modSource->getModulationValue() * (modRange / 2.f);
    current.setFloatValue(fCurrent + fModValue);
}
