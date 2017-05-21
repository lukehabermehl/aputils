//
//  autil_osc_private.hpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_osc_private_h
#define autil_osc_private_h

#include "autil_osc.hpp"

class APUTrivialOscillator::Pimpl
{
public:
    Pimpl()
    : freqHz(440)
    , sampleRate(44100)
    , inc(0)
    , modulo(0)
    , pulseWidth(0.5)
    , waveform(SAW)
    {}
    double freqHz;
    double sampleRate;
    double inc;
    double modulo;
    double pulseWidth;

    Waveform waveform;

    float getNextSample();
    void updateInc();
};


#endif /* autil_osc_private_h */
