//
//  portaudio_kernel.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "portaudio_host.hpp"
#include "autil_logger.hpp"

PortAudioKernel::PortAudioKernel()
: audioProcessingUnit(NULL)
, numInputChannels(1)
, numOutputChannels(1)
, stream(NULL)
, inputMode(AudioInputModeNone)
, status(APUHostInterface::DONE)
, streamStatusChangeCallback(NULL)
, streamStatusChangeCallbackCtx(NULL)
{
    passthroughUnit = AudioProcessingUnit::createPassthroughUnit();
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


    if (inputMode == AudioInputModeDevice) {
        sampleRate = audioFile->sampleRate();
    }
    else
    {
        //TODO: use set inputDeviceIndex
        const PaDeviceInfo *inputDevInfo = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
        sampleRate = inputDevInfo->defaultSampleRate;
        inputParameters.channelCount = inputDevInfo->maxInputChannels;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.device = Pa_GetDefaultInputDevice();
        inputParameters.suggestedLatency = inputDevInfo->defaultLowInputLatency;
    }

    APUGetLogger()->log(kPortAudioKernelLogPrefix, LOG_LEVEL_DEBUG,
                        "Open stream with sample rate: %lu",
                        sampleRate);

    const PaStreamParameters *pParams = (inputMode == AudioInputModeFile) ? NULL : &inputParameters;

    paError = Pa_OpenStream(&stream,
                            pParams,
                            &outputParameters,
                            sampleRate,
                            1,
                            0,
                            &PortAudioKernel::paCallback,
                            this);

    if (paError != paNoError)
    {
        APUGetLogger()->log(kPortAudioKernelLogPrefix,
                            LOG_LEVEL_ERROR,
                            "Failed to open stream! PAError code: %d",
                            paError);
        return false;
    }

    paError = Pa_SetStreamFinishedCallback(stream, &PortAudioKernel::paStreamFinished);
    if (paError != paNoError)
    {
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
    if (inputMode != AudioInputModeDevice) {
        in = (float *)inputBuffer;
    }

    PaStreamCallbackResult ret = paContinue;

    for (unsigned long i=0; i<framesPerBuffer; i++)
    {
        if (inputMode == AudioInputModeFile) {
            if (audioFile->nextFrame(&in) != AudioFileBufferStatusOK) {
                ret = paComplete;
                break;
            }
        }

        //If no APU, passthrough
        if (!audioProcessingUnit)
        {
            passthroughUnit->processAudio(in, out, numInputChannels, numOutputChannels);
        }

        else
        {
            audioProcessingUnit->processAudio(in, out, numInputChannels, numOutputChannels);
        }

    }

    return ret;
}

void PortAudioKernel::paStreamFinished(void *userData)
{
    return ((PortAudioKernel *)userData)->paStreamFinishedMethod();
}




