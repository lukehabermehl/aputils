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
    _pimpl = new pimpl;
    _pimpl->dspKernel = new PortAudioKernel();
    _pimpl->dspKernel->streamStatusChangeCallback = APUPortAudioHost::streamStatusChangeCallback;
    _pimpl->dspKernel->streamStatusChangeCallbackCtx = this;
    _pimpl->outputDeviceIndex = 1;
    _pimpl->eventSinks = new APUEnumerator<APUHostEventSink>();
    
    Pa_Initialize();
}

APUPortAudioHost::~APUPortAudioHost()
{
    delete _pimpl;
    Pa_Terminate();
}

void APUPortAudioHost::setNumOutputChannels(int numOutputChannels)
{
    _pimpl->dspKernel->numOutputChannels = numOutputChannels;
}

int APUPortAudioHost::getNumOutputChannels() const
{
    return _pimpl->dspKernel->numOutputChannels;
}

void APUPortAudioHost::setInputMode(AudioInputMode mode)
{
    _pimpl->dspKernel->inputMode = mode;
}

AudioDeviceIndex APUPortAudioHost::getInputDevice() const
{
    return _pimpl->inputDeviceIndex;
}

AudioDeviceIndex APUPortAudioHost::getOutputDevice() const
{
    return _pimpl->outputDeviceIndex;
}

void APUPortAudioHost::setInputDevice(AudioDeviceIndex devIndex)
{
    _pimpl->inputDeviceIndex = devIndex;
}

void APUPortAudioHost::setOutputDevice(AudioDeviceIndex devIndex)
{
    _pimpl->outputDeviceIndex = devIndex;
}

APUObjRet<APUEnumerable<AudioDevice> >
APUPortAudioHost::getDevices() const
{
    //Scan for audio devices
    //BDLog(kAPUPortAudioHostLogPrefix, "Scan for audio devices");
    int devCount = Pa_GetDeviceCount();
    APUPtr<APUEnumerator<AudioDevice> > devices = new APUEnumerator<AudioDevice>();
    
    for (int i=0; i<devCount; i++)
    {
        const PaDeviceInfo *info = Pa_GetDeviceInfo(i);
        APUPtr<AudioDevice> dev = new AudioDevice(i, info->name);
        devices->addObject(dev);
    }
    
    return (APUEnumerable<AudioDevice> *)devices.ptr();
}

APUHostInterface::ErrorCode APUPortAudioHost::getError() const
{
    switch (_pimpl->dspKernel->paError)
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
    _pimpl->dspKernel->audioProcessingUnit = unit;
    if (unit == NULL) {
        APUGetLogger()->log(kAPUPortAudioHostLogPrefix,
                            LOG_LEVEL_ERROR,
                            "APUnit is null!");
        return;
    }
    unit->setSampleRate(_pimpl->dspKernel->sampleRate);
}

bool APUPortAudioHost::initialize()
{
    return _pimpl->dspKernel->open(getOutputDevice());
}

bool APUPortAudioHost::destroy()
{
    if (_pimpl->dspKernel->close())
    {
        return true;
    }
    
    return false;
}

bool APUPortAudioHost::start()
{
    _pimpl->dspKernel->audioProcessingUnit->setupInitialState();
    return _pimpl->dspKernel->start();
}

bool APUPortAudioHost::stop()
{
    return _pimpl->dspKernel->stop();
}

bool APUPortAudioHost::setInputFile(AudioFile *file)
{
    if (file == NULL || file->mode() == AudioFileModeWriteOnly)
        return false;
    
    _pimpl->dspKernel->audioFile = file;
    
    return true;
}

APUObjRet<AudioFile> APUPortAudioHost::getInputFile() const
{
    return _pimpl->dspKernel->audioFile;
}

APUHostInterface::Status APUPortAudioHost::getStatus() const
{
    return _pimpl->dspKernel->status;
}

void APUPortAudioHost::sleep(unsigned long millisec)
{
    Pa_Sleep(millisec);
}

void APUPortAudioHost::setLooping(bool looping)
{
    _pimpl->dspKernel->audioFile->setLooping(looping);
}

void APUPortAudioHost::subscribe(APUHostEventSink *eventSink)
{
    if (eventSink) {
        _pimpl->eventSinks->addObject(eventSink);
    }
}

void APUPortAudioHost::unsubscribe(APUHostEventSink *eventSink)
{
    if (eventSink && _pimpl->eventSinks->size()) {
        _pimpl->eventSinks->removeObject(eventSink);
    }
}

void APUPortAudioHost::streamStatusChangeCallback(void *ctx)
{
    APUPortAudioHost *audioManager = (APUPortAudioHost *)ctx;
    if (audioManager->_pimpl->eventSinks->size()) {
        audioManager->_pimpl->eventSinks->reset();
        do {
            audioManager->_pimpl->eventSinks->getCurrent()->handleEvent(APUHOST_EVENT_STATUSCHANGED,
                                                                        audioManager);
        } while (audioManager->_pimpl->eventSinks->moveNext());
    }
}

APUPortAudioHost::pimpl::~pimpl()
{
    assert(eventSinks->size() == 0);
    delete dspKernel;
}
