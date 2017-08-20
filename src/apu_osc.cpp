//
//  apu_osc.cpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include "apu_osc.hpp"
#include "apu_osc_private.hpp"

#include "apu_logger.hpp"

APUTrivialOscillator::APUTrivialOscillator()
{
    pimpl_ = new Pimpl();
}

APUTrivialOscillator::~APUTrivialOscillator()
{
    delete pimpl_;
}

void
APUTrivialOscillator::setWaveform(APUTrivialOscillator::Waveform waveform)
{
    pimpl_->waveform = waveform;
}

void
APUTrivialOscillator::setPulseWidth(double pulseWidthPct)
{
    pimpl_->pulseWidth = pulseWidthPct;
}

void
APUTrivialOscillator::setFrequency(double freqHz)
{
    pimpl_->freqHz = freqHz;
    pimpl_->updateInc();
}

double
APUTrivialOscillator::getFrequency()
{
    return pimpl_->freqHz;
}

float
APUTrivialOscillator::getNextSample()
{
    return pimpl_->getNextSample();
}

void APUTrivialOscillator::setSampleRate(double sampleRate)
{
    pimpl_->sampleRate = sampleRate;
    pimpl_->updateInc();
}

float
APUTrivialOscillator::Pimpl::getNextSample()
{
    modulo += inc;
    if (modulo >= 1.0) {
        modulo -= 1.0;
    }

    switch (waveform) {
        case SAW:
        {
            return 2.0 * modulo - 1.0;
        }
        case SQUARE:
        {
            return modulo > pulseWidth ? 0 : 1;
        }
    }
}

void
APUTrivialOscillator::Pimpl::updateInc()
{
    inc = freqHz / sampleRate;
}

//---------------------------------------------------------
// APUOscModSource
//---------------------------------------------------------

APUOscModSource::APUOscModSource(APUWaveSource *waveSource)
{
    m_pimpl = new Pimpl();
    m_pimpl->waveSource = waveSource;
}

APUOscModSource::~APUOscModSource()
{
    delete m_pimpl;
}

float
APUOscModSource::getModulationValue()
{
    return m_pimpl->currentValue;
}

void
APUOscModSource::next()
{
    m_pimpl->currentValue = m_pimpl->waveSource->getNextSample();
}

void
APUOscModSource::setSampleRate(double sampleRate)
{
    m_pimpl->waveSource->setSampleRate(sampleRate);
}

void
APUOscModSource::setFrequency(double freqHz)
{
    m_pimpl->waveSource->setFrequency(freqHz);
}

double
APUOscModSource::getFrequency()
{
    return m_pimpl->waveSource->getFrequency();
}

//---------------------------------------------------------
// APUWaveTableOscillator
//---------------------------------------------------------

APUWaveTableOscillator::APUWaveTableOscillator()
{
    m_pimpl = new Pimpl();
    m_pimpl->waveform = SINE;
    m_pimpl->index = 0;
}

APUWaveTableOscillator::~APUWaveTableOscillator()
{
    delete m_pimpl;
}

void
APUWaveTableOscillator::setWaveform(APUWaveTableOscillator::Waveform waveform)
{
    m_pimpl->waveform = waveform;
}

void
APUWaveTableOscillator::setFrequency(double freqHz)
{
    m_pimpl->freqHz = freqHz;
    m_pimpl->update();
}

double
APUWaveTableOscillator::getFrequency()
{
    return m_pimpl->freqHz;
}

float
APUWaveTableOscillator::getNextSample()
{
    return m_pimpl->getNextSample();
}

void
APUWaveTableOscillator::setSampleRate(double sampleRate)
{
    m_pimpl->sampleRate = sampleRate;
    m_pimpl->update();
}

void
APUWaveTableOscillator::Pimpl::update()
{
    inc = (float)WAVE_TABLE_SIZE * (freqHz / (float)sampleRate);
}

float
APUWaveTableOscillator::Pimpl::getNextSample()
{
    index += inc;
    if (index >= (double)WAVE_TABLE_SIZE) {
        index -= (double)WAVE_TABLE_SIZE;
    }

    return SineWaveTable::getSingleton().getAt(index);
}
