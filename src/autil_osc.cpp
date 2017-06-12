//
//  autil_osc.cpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include "autil_osc.hpp"
#include "autil_osc_private.hpp"

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
            return modulo > pulseWidth ? -1 : 1;
        }
    }
}

void
APUTrivialOscillator::Pimpl::updateInc()
{
    inc = freqHz / sampleRate;
}