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
#include "autil_mod.hpp"

/** Interface for objects that generate waveforms */
class APUWaveSource
: public APUObjectInterface
{
public:
    /** Set the center frequency */
    virtual void setFrequency(double freqHz) = 0;
    /** Get the center frequency */
    virtual double getFrequency() = 0;
    /** Get the next sample in the waveform */
    virtual float getNextSample() = 0;
    /** Set the sampling rate */
    virtual void setSampleRate(double sampleRate) = 0;

    virtual ~APUWaveSource() {}
};

/** Modulation source implementation using an oscillator */
class APUOscModSource
: public APUModSource
, public APUObject
{
public:
    APUOBJ_FWDDECL

    APUOscModSource(APUWaveSource *waveSource);
    virtual ~APUOscModSource();

    // APUModSource
    virtual float getModulationValue();
    virtual void next();

private:
    class Pimpl;
    Pimpl *m_pimpl;
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

class APUWaveTableOscillator
: public APUWaveSource
, public APUObject
{
public:
    APUOBJ_FWDDECL

    APUWaveTableOscillator();
    virtual ~APUWaveTableOscillator();

    enum Waveform {
        SINE
    };

    virtual void setWaveform(Waveform waveform);

    virtual void setFrequency(double freqHq);
    virtual double getFrequency();
    virtual float getNextSample();
    virtual void setSampleRate(double sampleRate);

private:
    class Pimpl;
    Pimpl *m_pimpl;
};

#endif /* autil_osc_h */
