//
//  autil_osc_private.hpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_osc_private_h
#define autil_osc_private_h

#include <cmath>
#include <cassert>

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

class APUOscModSource::Pimpl
{
public:
    APUPtr<APUWaveSource> waveSource;
    float currentValue;
};

static const size_t WAVE_TABLE_SIZE = 1024;

class SineWaveTable
{
private:
    float *m_table;
    int m_refCount;

    SineWaveTable()
    : m_table(NULL)
    , m_refCount(0)
    {
    }
public:
    virtual ~SineWaveTable() {
        if (m_table != NULL) {
            delete []m_table;
        }
    }
    static SineWaveTable &getSingleton() {
        static SineWaveTable singleton;
        return singleton;
    }

    void retain() {
        if (m_refCount == 0) {
            m_table = new float[1024];
            float twoPi = M_PI * 2.f;
            float interval = twoPi / 1023.f;
            for (size_t i=0; i<1024; i++) {
                m_table[i] = sin(interval * i);
            }
        }
        ++m_refCount;
    }

    void release() {
        if (m_refCount > 0) {
            --m_refCount;
            if (m_refCount == 0) {
                delete []m_table;
                m_table = NULL;
            }
        }
    }

    inline float getAt(double index) {
        assert(index < WAVE_TABLE_SIZE);
        assert(m_table != NULL);

        size_t low = floor(index);
        size_t high = ceil(index);

        double slope = (m_table[high] - m_table[low]) / ((double)high - (double)low);

        return m_table[low] + ((index - (double)low) * slope);
    }
};

class APUWaveTableOscillator::Pimpl
{
public:
    Pimpl() {
        SineWaveTable::getSingleton().retain();
    }
    ~Pimpl() {
        SineWaveTable::getSingleton().release();
    }
    double sampleRate;
    double freqHz;
    double inc;

    double index;

    void update();
    float getNextSample();

    Waveform waveform;
};


#endif /* autil_osc_private_h */
