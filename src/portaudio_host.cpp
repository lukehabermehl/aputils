//
//  portaudio_host.cpp
//  libaputils
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "apu_logger.hpp"
#include "portaudio_host.hpp"

#include <memory>

PortAudioKernel::PortAudioKernel()
: audioProcessingUnit(NULL)
, numInputChannels(1)
, numOutputChannels(1)
, stream(NULL)
, inputMode(APUPortAudioHost::INPUT_NONE)
, status(APUHostInterface::DONE)
, streamStatusChangeCallback(NULL)
, streamStatusChangeCallbackCtx(NULL)
{
    APUPtr<AudioProcessingUnit> ptUnit = AudioProcessingUnit::createPassthroughUnit();
    passthroughUnit = ptUnit;
}

PortAudioKernel::~PortAudioKernel()
{
    if (stream) {
        close();
    }
}

bool PortAudioKernel::open(PaDeviceIndex outputDevIndex)
{
    PaStreamParameters outputParameters;
    outputParameters.device = outputDevIndex;
    if (outputParameters.device == paNoDevice)
    {
        APUGetLogger()->log(kPortAudioKernelLogPrefix,
                            LOG_LEVEL_ERROR,
                            "PortAudioKernel Failed to find output device!");
        return false;
    }

    outputParameters.channelCount = numOutputChannels;
    outputParameters.sampleFormat = paFloat32;
    const PaDeviceInfo *devInfo = Pa_GetDeviceInfo(outputParameters.device);
    outputParameters.suggestedLatency = devInfo->defaultLowOutputLatency;

    outputParameters.hostApiSpecificStreamInfo = NULL;

    PaStreamParameters inputParameters;

    switch (inputMode) {
        case APUPortAudioHost::INPUT_NONE:
            if (audioProcessingUnit) {
                sampleRate = audioProcessingUnit->getPreferredSampleRate();
            } else {
                sampleRate = 44100;
            }
            break;
        case APUPortAudioHost::INPUT_FILE:
            sampleRate = audioFile->sampleRate();
            numInputChannels = (int)audioFile->numChannels();
            break;
        case APUPortAudioHost::INPUT_DEVICE:
            //TODO: use set inputDeviceIndex
            const PaDeviceInfo *inputDevInfo = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
            sampleRate = inputDevInfo->defaultSampleRate;
            numInputChannels = inputDevInfo->maxInputChannels;
            inputParameters.sampleFormat = paFloat32;
            inputParameters.device = Pa_GetDefaultInputDevice();
            inputParameters.suggestedLatency = inputDevInfo->defaultLowInputLatency;
            break;
    }

    inputParameters.channelCount = numInputChannels;

    APUGetLogger()->log(kPortAudioKernelLogPrefix, LOG_LEVEL_DEBUG,
                        "Open stream with sample rate: %lu",
                        sampleRate);

    const PaStreamParameters *pParams = (inputMode != APUPortAudioHost::INPUT_DEVICE) ? NULL : &inputParameters;

    paError = Pa_OpenStream(&stream,
                            pParams,
                            &outputParameters,
                            sampleRate,
                            256,
                            0,
                            &PortAudioKernel::paCallback,
                            this);

    if (paError != paNoError) {
        APUGetLogger()->log(kPortAudioKernelLogPrefix,
                            LOG_LEVEL_ERROR,
                            "Failed to open stream! PAError code: %d",
                            paError);
        return false;
    }

    paError = Pa_SetStreamFinishedCallback(stream, &PortAudioKernel::paStreamFinished);
    if (paError != paNoError) {
        Pa_CloseStream(stream);
        stream = 0;
        return false;
    }

    if (audioProcessingUnit) {
        audioProcessingUnit->setSampleRate(sampleRate);
    }

    return true;
}

bool PortAudioKernel::close()
{
    if (stream == 0)
        return false;

    paError = Pa_CloseStream(stream);
    stream = 0;

    return (paError == paNoError);
}

bool PortAudioKernel::start()
{
    if (stream == 0)
        return false;

    doStop = false;

    paError = Pa_StartStream(stream);
    status = APUHostInterface::RUNNING;
    if (streamStatusChangeCallback) {
        streamStatusChangeCallback(streamStatusChangeCallbackCtx);
    }
    return (paError == paNoError);
}

bool PortAudioKernel::stop()
{
    if (stream == 0)
        return false;

    doStop = true;
    return true;
}

void PortAudioKernel::paStreamFinishedMethod()
{
    status = APUHostInterface::DONE;
    close();
    if (streamStatusChangeCallback) {
        streamStatusChangeCallback(streamStatusChangeCallbackCtx);
    }
}

int PortAudioKernel::paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo,
                     PaStreamCallbackFlags statusFlags)
{
    if (doStop) return paComplete;

    float *out = (float *)outputBuffer;
    float *in = 0;
    uint32_t numFrames = (uint32_t)framesPerBuffer;
    if (inputMode != APUPortAudioHost::INPUT_DEVICE) {
        in = (float *)inputBuffer;
    }

    PaStreamCallbackResult ret = paContinue;

    if (inputMode == APUPortAudioHost::INPUT_FILE) {
        float fileInput[2048];
        memset(fileInput, 0, 2048 * sizeof(float));
        size_t samplesRead = 0;
        AudioFile::BufferStatus fileStatus = audioFile->read(fileInput, 256, samplesRead);
        in = fileInput;
        if (fileStatus != AudioFile::STATUS_OK) {
            ret = paComplete;
        }
        numFrames = (uint32_t)samplesRead / numInputChannels;
    }
    else if (inputMode == APUPortAudioHost::INPUT_DEVICE) {
        in = (float *)inputBuffer;
    }

    out = (float *)outputBuffer;

    AudioProcessingUnit *unit = audioProcessingUnit ? audioProcessingUnit : passthroughUnit;
    unit->processAudio(in, out, numInputChannels, numOutputChannels, numFrames);

    return ret;
}

void PortAudioKernel::paStreamFinished(void *userData)
{
    return ((PortAudioKernel *)userData)->paStreamFinishedMethod();
}




