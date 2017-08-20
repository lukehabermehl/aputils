//
//  apu_osc.hpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_osc_h
#define apu_osc_h

#include "apu_obj.hpp"
#include "apu_mod.hpp"

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
    /** Set the sample rate. Must be called before getNextSample() */
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

    /** Construct the modulation source using the given wave source.
      * This object will use the wave source internally to generate modulation values
      */
    APUOscModSource(APUWaveSource *waveSource);
    virtual ~APUOscModSource();

    /** Set the sample rate of the internal wave source */
    virtual void setSampleRate(double sampleRate);
    /** Set the frequency of the internal wave source */
    virtual void setFrequency(double freqHz);
    /** Get the frequency of the internal wave source */
    virtual double getFrequency();

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

/** Oscillator that uses a wave table to lookup values */
class APUWaveTableOscillator
: public APUWaveSource
, public APUObject
{
public:
    APUOBJ_FWDDECL

    APUWaveTableOscillator();
    virtual ~APUWaveTableOscillator();

    /** Available waveforms for this type of oscillator */
    enum Waveform {
        SINE
    };

    /** Set the waveform to generate */
    virtual void setWaveform(Waveform waveform);

    /** Set the center frequency (pitch) in Hertz */
    virtual void setFrequency(double freqHz);
    /** Get the center frequency (pitch) */
    virtual double getFrequency();
    /** Get the next sample value for the waveform at the current frequency */
    virtual float getNextSample();
    /** Set the sample rate. This must be set before calling getNextSample() */
    virtual void setSampleRate(double sampleRate);

private:
    class Pimpl;
    Pimpl *m_pimpl;
};

#endif /* apu_osc_h */
