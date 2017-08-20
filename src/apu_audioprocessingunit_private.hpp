//
//  apu_audioprocessingunit_private.hpp
//  aputils
//
//  Created by Luke on 8/28/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef apu_audioprocessingunit_private_h
#define apu_audioprocessingunit_private_h

#include "apu_audioprocessingunit.hpp"
#include "apu_parameter.hpp"
#include <unordered_map>
#include <string>

class AudioProcessingUnit::Pimpl
{
public:
    Pimpl() :sampleRate(0)
    {
        parameterMap = new APUParameterMap();
    }
    size_t sampleRate;

    APUPtr<APUParameterMap> parameterMap;
};


#endif /* apu_audioprocessingunit_private_h */
