//
//  dsptest.cpp
//  libaputils
//
//  Created by Luke on 5/20/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include <aputils/aputils.h>

class SimpleGainUnit
: public AudioProcessingUnit
{
public:
    virtual void processAudio(float *inputBuffer, float *outputBuffer,
                              int numInputChannels, int numOutputChannels)
    {
        outputBuffer[0] = inputBuffer[0] * 0.707;
        if (numOutputChannels == 2) {
            if (numInputChannels == 2) {
                outputBuffer[1] = inputBuffer[1] * 0.707;
            } else {
                outputBuffer[1] = outputBuffer[0];
            }
        }
    }

    virtual const char * getName() {
        return "SimpleGainUnit";
    }

    virtual void setupInitialState() {
        /* empty */
    }
};

int main()
{
    APUGetLogger()->setLogLevel(LOG_LEVEL_DEBUG);
    APUPtr<APUPortAudioHost> audioManager = new APUPortAudioHost();
    audioManager->setInputMode(AudioInputModeFile);
    audioManager->setInputDevice(-1);
    audioManager->setOutputDevice(1);
    audioManager->setNumOutputChannels(2);

    APUPtr<AudioFile> file = new AudioFile("/Users/Luke/Projects/aputils/test/dsptest/guitar.wav",
                                           AudioFileModeReadOnly);

    APUPtr<AudioProcessingUnit> unit = new SimpleGainUnit();
    audioManager->setInputFile(file);
    audioManager->setAudioProcessingUnit(unit);
    audioManager->initialize();
    audioManager->setLooping(false);
    audioManager->start();
    audioManager->sleep(5000);
    audioManager->stop();
    audioManager->destroy();
}


