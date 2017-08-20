//
//  AudioProcessingUnit.cpp
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "apu_audioprocessingunit.hpp"
#include "apu_audioprocessingunit_private.hpp"

AudioProcessingUnit::AudioProcessingUnit()
{
    m_pimpl = new Pimpl();
}

AudioProcessingUnit::~AudioProcessingUnit()
{
    delete m_pimpl;
}

void AudioProcessingUnit::setupInitialState()
{
}

void AudioProcessingUnit::processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels)
{
    outputBuffer[0] = inputBuffer[0];
    
    if (numInputChannels == 1 && numOutputChannels == 2)
    {
        outputBuffer[1] = outputBuffer[0];
    }
    
    else if (numInputChannels == 1 && numOutputChannels == 2)
    {
        outputBuffer[1] = inputBuffer[1];
    }
}

APUObjRet<AudioProcessingUnit>
AudioProcessingUnit::createPassthroughUnit()
{
    return new AudioProcessingUnit;
}

void AudioProcessingUnit::setMaxInputChannels(uint32_t num)
{
}

void AudioProcessingUnit::setMaxOutputChannels(uint32_t num)
{
}

void AudioProcessingUnit::setSampleRate(unsigned long sampleRate)
{
    m_pimpl->sampleRate = sampleRate;

    for (APUParameterMap::Iterator it = m_pimpl->parameterMap->begin(); it.valid(); ++it) {
        it.second()->setSampleRate(sampleRate);
    }
}

unsigned long AudioProcessingUnit::getSampleRate()
{
    return m_pimpl->sampleRate;
}

unsigned long AudioProcessingUnit::getPreferredSampleRate()
{
    return 44100;
}

APUObjRet<APUString> AudioProcessingUnit::getName()
{
    static APUStringRef defaultName = "Unnamed APU";
    return defaultName.get();
}

size_t AudioProcessingUnit::getNumParameters()
{
    return m_pimpl->parameterMap->size();
}

APUObjRet<APUParameter> AudioProcessingUnit::getParameterWithIdentifier(const char *identifier)
{
    return m_pimpl->parameterMap->parameterWithIdentifier(identifier);
}

const APUParameterMap * AudioProcessingUnit::getParameterMap()
{
    return m_pimpl->parameterMap.ptr();
}

bool AudioProcessingUnit::addParameter(APUParameter *param)
{
    APUStringRef paramId = param->getIdentifier();
    if (!paramId.isEmpty()) {
        APUPtr<APUParameter> existing = getParameterWithIdentifier(paramId);
        if (!existing) {
            m_pimpl->parameterMap->addParameter(param);
            return true;
        }
    }

    return false;
}

