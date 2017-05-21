//
//  autil_osc.hpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_osc_h
#define autil_osc_h

#include "autil_obj.hpp"

class APUWaveSource
: public APUObjectInterface
{
    virtual void setFrequency(double freqHz) = 0;
    virtual double getFrequency() = 0;
    virtual float getNextSample() = 0;
    virtual void setSampleRate(double sampleRate) = 0;
};

class APUTrivialOscillator
: public APUWaveSource
, private APUObject
{
public:
    APUOBJ_FWDDECL

    enum Waveform {
        SAW,
        SQUARE
    };

    APUTrivialOscillator();
    virtual ~APUTrivialOscillator();

    virtual void setWaveform(Waveform waveform);
    virtual void setPulseWidth(double pulseWidthPct);

    // APUWaveSource
    virtual void setFrequency(double freqHz);
    virtual double getFrequency();
    virtual float getNextSample();
    virtual void setSampleRate(double sampleRate);

private:
    class Pimpl;
    Pimpl *pimpl_;
};

#endif /* autil_osc_h */
