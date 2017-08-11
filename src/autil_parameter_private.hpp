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
#include <atomic>
#include <string>

class APUParameter::Pimpl
{
public:
    Pimpl(APUString *pname, APUParameterCallback *callbck)
    :isSmoothing(false), smoothingEnabled(false), smoothingInterval_msec(0), smoothingFrames(0)
    ,name(pname), modRange(0)
    ,cb(NULL)
    {
        target = 0;
        current = 0;
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

    std::atomic<APUNumber> target;
    std::atomic<APUNumber> current;
    std::atomic<APUNumber> base;
    std::atomic<APUNumber> minValue;
    std::atomic<APUNumber> maxValue;
    float modRange;
    APUPtr<APUModSource> modSource;
    float diffPerUpdate;

    APUParameterCallback *cb;
};

class APUEnumParameter::EnumParamPimpl
{
public:
    APUPtr<APUArray<APUString> > strings;
};


#endif /* autil_parameter_private_h */
