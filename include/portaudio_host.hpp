//
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file portaudio_host.hpp 
  * Internal interface to PortAudio API
  */

#ifndef PortAudioKernel_hpp
#define PortAudioKernel_hpp

#include <portaudio.h>

#include "autil_file.hpp"
#include "autil_audioprocessingunit.hpp"
#include "autil_manager.hpp"

static const char *kPortAudioKernelLogPrefix = "[PortAudioKernel]";

class PortAudioKernel {
public:
    typedef void (*StreamStatusChangeCallback)(void *);
    PortAudioKernel();
    ~PortAudioKernel();
    
    bool open(PaDeviceIndex outputDevIndex);
    bool close();
    bool start();
    bool stop();
    
    void paStreamFinishedMethod();
    
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags);

    static int paCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
    {
        PortAudioKernel *kernel = (PortAudioKernel *)userData;
        return kernel->paCallbackMethod(inputBuffer,
                                        outputBuffer,
                                        framesPerBuffer,
                                        timeInfo,
                                        statusFlags);
    }
    
    static void paStreamFinished(void *userData);
    
    int numOutputChannels;
    int numInputChannels;
    unsigned long sampleRate;
    PaStream *stream;
    AudioInputMode inputMode;
    APUPtr<AudioFile> audioFile;
    APUPtr<AudioProcessingUnit> audioProcessingUnit;
    AudioManagerStatus status;
    StreamStatusChangeCallback streamStatusChangeCallback;
    PaError paError;
    void *streamStatusChangeCallbackCtx;
    
private:
    APUPtr<AudioProcessingUnit> passthroughUnit;
    bool doStop;
};


#endif /* PortAudioKernel_hpp */
