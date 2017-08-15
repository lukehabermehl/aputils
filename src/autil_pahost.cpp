//
//  APUPortAudioHost.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_pahost.hpp"
#include "autil_pahost_private.hpp"
#include "autil_file.hpp"
#include "autil_logger.hpp"

#include <portaudio.h>
#include <cassert>

static const char *kAPUPortAudioHostLogPrefix = "[APUPortAudioHost]";

APUPortAudioHost::APUPortAudioHost()
{
    m_pimpl = new Pimpl;
    m_pimpl->dspKernel = new PortAudioKernel();
    m_pimpl->dspKernel->streamStatusChangeCallback = APUPortAudioHost::streamStatusChangeCallback;
    m_pimpl->dspKernel->streamStatusChangeCallbackCtx = this;
    m_pimpl->outputDeviceIndex = 1;
    m_pimpl->eventSinks = new APUEnumerator<APUHostEventSink>();
    
    Pa_Initialize();
}

APUPortAudioHost::~APUPortAudioHost()
{
    delete m_pimpl;
    Pa_Terminate();
}

void APUPortAudioHost::setNumOutputChannels(int numOutputChannels)
{
    m_pimpl->dspKernel->numOutputChannels = numOutputChannels;
}

int APUPortAudioHost::getNumOutputChannels() const
{
    return m_pimpl->dspKernel->numOutputChannels;
}

void APUPortAudioHost::setInputMode(AudioInputMode mode)
{
    m_pimpl->dspKernel->inputMode = mode;
}

AudioDeviceIndex APUPortAudioHost::getInputDevice() const
{
    return m_pimpl->inputDeviceIndex;
}

AudioDeviceIndex APUPortAudioHost::getOutputDevice() const
{
    return m_pimpl->outputDeviceIndex;
}

void APUPortAudioHost::setInputDevice(AudioDeviceIndex devIndex)
{
    m_pimpl->inputDeviceIndex = devIndex;
}

void APUPortAudioHost::setOutputDevice(AudioDeviceIndex devIndex)
{
    m_pimpl->outputDeviceIndex = devIndex;
}

APUObjRet<APUEnumerable<AudioDevice> >
APUPortAudioHost::getDevices() const
{
    //Scan for audio devices
    int devCount = Pa_GetDeviceCount();
    APUPtr<APUEnumerator<AudioDevice> > devices = new APUEnumerator<AudioDevice>();
    
    for (int i=0; i<devCount; i++) {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        APUPtr<AudioDevice> dev = new AudioDevice(i, APUStringMake(info->name));
        devices->addObject(dev);
    }
    
    return (APUEnumerable<AudioDevice> *)devices.ptr();
}

APUHostInterface::ErrorCode APUPortAudioHost::getError() const
{
    switch (m_pimpl->dspKernel->paError)
    {
        case paNoError:
            return APUHostInterface::OK;
        case paNotInitialized:
            return APUHostInterface::UNINITIALIZED;
        case paInvalidChannelCount:
            return APUHostInterface::INVALID_CHANNEL_COUNT;
        case paInvalidSampleRate:
            return APUHostInterface::INVALID_SAMPLE_RATE;
        case paInvalidDevice:
            return APUHostInterface::INVALID_DEVICE;
        case paDeviceUnavailable:
            return APUHostInterface::DEVICE_UNAVAILABLE;
        default:
            return APUHostInterface::UNKNOWN;
    }
}

void APUPortAudioHost::setAudioProcessingUnit(AudioProcessingUnit *unit)
{
    m_pimpl->dspKernel->audioProcessingUnit = unit;
    if (unit == NULL) {
        APUGetLogger()->log(kAPUPortAudioHostLogPrefix,
                            LOG_LEVEL_ERROR,
                            "APUnit is null!");
        return;
    }
}

bool APUPortAudioHost::initialize()
{
    return m_pimpl->dspKernel->open(getOutputDevice());
}

bool APUPortAudioHost::destroy()
{
    if (m_pimpl->dspKernel->close())
    {
        return true;
    }
    
    return false;
}

bool APUPortAudioHost::start()
{
    m_pimpl->dspKernel->audioProcessingUnit->setSampleRate(m_pimpl->dspKernel->sampleRate);
    m_pimpl->dspKernel->audioProcessingUnit->setupInitialState();
    return m_pimpl->dspKernel->start();
}

bool APUPortAudioHost::stop()
{
    return m_pimpl->dspKernel->stop();
}

bool APUPortAudioHost::setInputFile(AudioFile *file)
{
    if (file == NULL)
        return false;
    
    m_pimpl->dspKernel->audioFile = file;
    
    return true;
}

APUObjRet<AudioFile> APUPortAudioHost::getInputFile() const
{
    return m_pimpl->dspKernel->audioFile;
}

APUHostInterface::Status APUPortAudioHost::getStatus() const
{
    return m_pimpl->dspKernel->status;
}

void APUPortAudioHost::sleep(unsigned long millisec)
{
    Pa_Sleep(millisec);
}

void APUPortAudioHost::setLooping(bool looping)
{
    m_pimpl->dspKernel->audioFile->setLooping(looping);
}

void APUPortAudioHost::subscribe(APUHostEventSink *eventSink)
{
    if (eventSink) {
        m_pimpl->eventSinks->addObject(eventSink);
    }
}

void APUPortAudioHost::unsubscribe(APUHostEventSink *eventSink)
{
    if (eventSink && m_pimpl->eventSinks->size()) {
        m_pimpl->eventSinks->removeObject(eventSink);
    }
}

void APUPortAudioHost::streamStatusChangeCallback(void *ctx)
{
    APUPortAudioHost *audioManager = (APUPortAudioHost *)ctx;
    if (audioManager->m_pimpl->eventSinks->size()) {
        audioManager->m_pimpl->eventSinks->reset();
        do {
            audioManager->m_pimpl->eventSinks->getCurrent()->handleEvent(APUStringMake(APUHOST_EVENT_STATUSCHANGED),
                                                                        audioManager);
        } while (audioManager->m_pimpl->eventSinks->moveNext());
    }
}

APUPortAudioHost::Pimpl::~Pimpl()
{
    assert(eventSinks->size() == 0);
    delete dspKernel;
}
