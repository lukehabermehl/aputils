//
//  dsptest.cpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include <aputils/aputils.h>

class ModulatingOsc
: public APUWaveSource
, public APUObject
{
    APUPtr<APUWaveTableOscillator> m_osc;
    APUPtr<APUWaveTableOscillator> m_modOsc;
    APUPtr<APUOscModSource> m_modSrc;
    APUPtr<APUParameter> m_freqParam;
public:
    APUOBJ_FWDDECL
    ModulatingOsc() {
        m_osc = new APUWaveTableOscillator();
        m_osc->setFrequency(440);
        m_modOsc = new APUWaveTableOscillator();
        m_modOsc->setFrequency(5);

        APUStringRef paramId = "Frequency";
        m_freqParam = new APUParameter(paramId,
                                       80,
                                       10000,
                                       800,
                                       NULL);

        m_freqParam->setTarget(800);
        m_freqParam->setModulationDepth(0.05);
        m_modSrc = new APUOscModSource(m_modOsc);
        m_freqParam->setModulationSource(m_modSrc);
    }

    void setFrequency(double freqHz) {
        m_osc->setFrequency(freqHz);
    }

    APUObjRet<APUParameter> getFrequencyParameter() { return m_freqParam; }

    double getFrequency() { return m_osc->getFrequency(); }

    float getNextSample() {
        m_modSrc->next();
        APUNumber freqVal = m_freqParam->getCurrentValue();
        if (freqVal.floatValue() != getFrequency()) {
            setFrequency(freqVal.floatValue());
        }
        return m_osc->getNextSample();
    }

    void setSampleRate(double sampleRate) {
        m_osc->setSampleRate(sampleRate);
        m_freqParam->setSampleRate(sampleRate);
        m_modOsc->setSampleRate(sampleRate);

        m_osc->setFrequency(m_freqParam->getTarget().floatValue());
    }

};

class TestAPU
: public AudioProcessingUnit
{
    APUPtr<ModulatingOsc> m_osc;
public:
    TestAPU() {
        m_osc = new ModulatingOsc();
    }

    void setupInitialState() {
        //m_osc->setSampleRate(getSampleRate());
    }

    void processAudio(float *inputBuffer, float *outputBuffer,
                      int numInputChannels, int numOutputChannels,
                      uint32_t numFrames)
    {
        for (uint32_t i = 0; i < numFrames; i++) {
            // float *pInput = &inputBuffer[i * numInputChannels];
            float *pOutput = &outputBuffer[i * numOutputChannels];

            pOutput[0] = m_osc->getNextSample();
            if (numOutputChannels > 1) {
                outputBuffer[1] = outputBuffer[0];
            }
        }
    }

    void setSampleRate(unsigned long sampleRate) {
        AudioProcessingUnit::setSampleRate(sampleRate);
        m_osc->setSampleRate(sampleRate);
    }
};


int main()
{
    APUPtr<APUPortAudioHost> host = new APUPortAudioHost();
    host->setInputMode(APUPortAudioHost::INPUT_NONE);
    host->setOutputDevice(1);
    APUPtr<AudioProcessingUnit> apu = new TestAPU();
    host->setAudioProcessingUnit(apu);
    host->initialize();
    host->start();
    host->sleep(5000);
    host->stop();
    host->destroy();
}


