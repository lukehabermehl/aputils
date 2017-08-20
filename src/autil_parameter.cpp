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

APUParameter::APUParameter(APUString *identifier, APUNumber minValue, APUNumber maxValue, APUNumber defaultValue, APUParameterCallback *cb)
{
    m_pimpl = new Pimpl(identifier, cb);
    setMinValue(minValue);
    setMaxValue(maxValue);
    m_pimpl->current = defaultValue;
    m_pimpl->target = defaultValue;
}

APUParameter::~APUParameter()
{
    delete m_pimpl;
}

APUObjRet<APUString>
APUParameter::getIdentifier()
{
    return m_pimpl->identifier.get();
}

APUObjRet<APUString> APUParameter::getName()
{
    return m_pimpl->name.get();
}

APUObjRet<APUString> APUParameter::getUnits()
{
    return m_pimpl->units.get();
}

void APUParameter::setUnits(APUString *units)
{
    m_pimpl->units = units;
}

APUNumberType APUParameter::type()
{
    return m_pimpl->target.load().getType();
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
    APUNumber currentValue = m_pimpl->current.load();
    normalizeValue(currentValue);
    m_pimpl->current.store(currentValue);
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
        m_pimpl->diffPerUpdate = (m_pimpl->target.load().floatValue() - m_pimpl->current.load().floatValue()) / (float)m_pimpl->smoothingFrames;
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

bool
APUParameter::supportsSmoothing()
{
    return type() == APUNUM_FLOAT;
}

void APUParameter::setSmoothingEnabled(bool enabled)
{
    m_pimpl->smoothingEnabled = supportsSmoothing() ? enabled : false;
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

void APUParameter::update()
{
    if (m_pimpl->isSmoothing) {
        float cv = m_pimpl->current.load().floatValue();
        cv += m_pimpl->diffPerUpdate;
        APUNumber currentValue = cv;
        m_pimpl->current.store(currentValue);
        m_pimpl->base.store(currentValue);
        
        if (cv == m_pimpl->target.load().floatValue()) {
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
            float fMinVal = m_pimpl->minValue.load().floatValue();
            float fMaxVal = m_pimpl->maxValue.load().floatValue();
            if (value.floatValue() < fMinVal) {
                value.setFloatValue(fMinVal);
                didNormalize = true;
            } else if (value.floatValue() > fMaxVal) {
                value.setFloatValue(fMaxVal);
                didNormalize = true;
            }
            break;
        }
        case APUNUM_INT:
        {
            int lMinVal = m_pimpl->minValue.load().integerValue();
            int lMaxVal = m_pimpl->maxValue.load().integerValue();
            if (value.integerValue() < lMinVal) {
                value.setIntegerValue(lMinVal);
                didNormalize = true;
            } else if (value.integerValue() > lMaxVal) {
                value.setIntegerValue(lMaxVal);
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
    m_pimpl->modRange = depth * (m_pimpl->maxValue.load().floatValue() - m_pimpl->minValue.load().floatValue());
}

void APUParameter::setModulationSource(APUModSource *source)
{
    m_pimpl->modSource = source;
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

bool
APUEnumParameter::supportsSmoothing()
{
    return false;
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

    float fCurrent = base.load().floatValue();
    float fModValue = modSource->getModulationValue() * (modRange / 2.f);
    APUNumber currentValue = fCurrent + fModValue;
    current.store(currentValue);
}
