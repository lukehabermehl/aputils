//
//  autil_parameter_private.hpp
//  libblockdsp
//
//  Created by Luke on 8/26/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_parameter_private_h
#define autil_parameter_private_h

#include "autil_parameter.hpp"
#include <string>

class APUParameter::Pimpl
{
public:
    Pimpl(APUNumberType vtype, APUString *pname, APUParameterCallback *callbck)
    :isSmoothing(false), smoothingEnabled(false), smoothingInterval_msec(0), smoothingFrames(0)
    ,name(pname), modRange(0), uiAttr(0), valueType(vtype)
    ,cb(NULL)
    {
        switch (valueType) {
            case APUNUM_FLOAT:
                target.setFloatValue(0);
                current.setFloatValue(0);
                break;
            case APUNUM_INTEGER:
                target.setIntegerValue(0);
                current.setIntegerValue(0);
                break;
            case APUNUM_UINT:
                target.setUnsignedIntValue(0);
                current.setUnsignedIntValue(0);
                break;
            case APUNUM_BOOLEAN:
                target.setBoolValue(false);
                current.setBoolValue(false);
        }
    }

    ~Pimpl()
    {
    }

    void doModulate();

    bool isSmoothing;
    bool smoothingEnabled;
    double smoothingInterval_msec;
    size_t smoothingFrames;

    APUPtr<APUString> name;
    APUPtr<APUString> units;

    APUNumber target;
    APUNumber current;
    APUNumber base;
    APUNumber minValue;
    APUNumber maxValue;
    float modRange;
    APUPtr<APUModSource> modSource;
    float diffPerUpdate;
    APUNumberType valueType;
    APUUIAttribute uiAttr;

    APUParameterCallback *cb;
};

class APUEnumParameter::EnumParamPimpl
{
public:
    APUPtr<APUArray<APUString> > strings;
};


#endif /* autil_parameter_private_h */
