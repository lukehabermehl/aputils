//
//  AudioProcessingUnit.cpp
//  libaputils
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "apu_audioprocessingunit.hpp"
#include "apu_audioprocessingunit_private.hpp"

#include <memory>

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

void AudioProcessingUnit::processAudio(float *inputBuffer, float *outputBuffer,
                                       int numInputChannels, int numOutputChannels,
                                       uint32_t numFrames)
{
    if (numInputChannels == numOutputChannels) {
        memcpy(outputBuffer, inputBuffer, numFrames * numInputChannels * sizeof(float));
    }
    else if (inputBuffer) {
        for (uint32_t i = 0; i < numFrames; i++) {
            float *pInput = &inputBuffer[i * numInputChannels];
            float *pOutput = &outputBuffer[i * numOutputChannels];

            pOutput[0] = pInput[0];
            if (numOutputChannels == 2) {
                pOutput[1] = pInput[0];
            }
        }
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

