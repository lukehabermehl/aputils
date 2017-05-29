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

/** Interface for objects that generate waveforms */
class APUWaveSource
: public APUObjectInterface
{
    /** Set the center frequency */
    virtual void setFrequency(double freqHz) = 0;
    /** Get the center frequency */
    virtual double getFrequency() = 0;
    /** Get the next sample in the waveform */
    virtual float getNextSample() = 0;
    /** Set the sampling rate */
    virtual void setSampleRate(double sampleRate) = 0;
};

/** A very basic oscillator that generates samples using a modulo counter */
class APUTrivialOscillator
: public APUWaveSource
, private APUObject
{
public:
    APUOBJ_FWDDECL

    /** Supported waveform types */
    enum Waveform {
        SAW,
        SQUARE
    };

    APUTrivialOscillator();
    virtual ~APUTrivialOscillator();

    /** Set the waveform type of the oscillator */
    virtual void setWaveform(Waveform waveform);
    /** Set the pulse width
      * @param pulseWidthPct the percentage (0.0 - 1.0) of the ON state compared to the OFF state
      */
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
