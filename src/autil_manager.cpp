//
//  AudioManager.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_manager.hpp"
#include "autil_manager_private.hpp"
#include "autil_file.hpp"
#include "autil_logger.hpp"

#include <portaudio.h>
#include <cassert>

static const char *kAudioManagerLogPrefix = "[AudioManager]";

AudioManager::AudioManager()
{
    _pimpl = new pimpl;
    _pimpl->dspKernel = new PortAudioKernel();
    _pimpl->dspKernel->streamStatusChangeCallback = AudioManager::streamStatusChangeCallback;
    _pimpl->dspKernel->streamStatusChangeCallbackCtx = this;
    _pimpl->outputDeviceIndex = 1;
    _pimpl->eventSinks = new APUEnumerator<APUHostEventSink>();
    
    Pa_Initialize();
}

AudioManager::~AudioManager()
{
    delete _pimpl;
    Pa_Terminate();
}

void AudioManager::setNumOutputChannels(int numOutputChannels)
{
    _pimpl->dspKernel->numOutputChannels = numOutputChannels;
}

int AudioManager::getNumOutputChannels() const
{
    return _pimpl->dspKernel->numOutputChannels;
}

void AudioManager::setInputMode(AudioInputMode mode)
{
    _pimpl->dspKernel->inputMode = mode;
}

AudioDeviceIndex AudioManager::getInputDevice()
{
    return _pimpl->inputDeviceIndex;
}

AudioDeviceIndex AudioManager::getOutputDevice() const
{
    return _pimpl->outputDeviceIndex;
}

void AudioManager::setInputDevice(AudioDeviceIndex devIndex)
{
    _pimpl->inputDeviceIndex = devIndex;
}

void AudioManager::setOutputDevice(AudioDeviceIndex devIndex)
{
    _pimpl->outputDeviceIndex = devIndex;
}

APUEnumerable<AudioDevice> *
AudioManager::getDevices()
{
    //Scan for audio devices
    //BDLog(kAudioManagerLogPrefix, "Scan for audio devices");
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

AudioManagerErrorCode AudioManager::getError() const
{
    switch (_pimpl->dspKernel->paError)
    {
        case paNoError:
            return AudioManagerOK;
        case paNotInitialized:
            return AudioManagerErrorUnintialized;
        case paInvalidChannelCount:
            return AudioManagerErrorInvalidChannelCount;
        case paInvalidSampleRate:
            return AudioManagerErrorInvalidSampleRate;
        case paInvalidDevice:
            return AudioManagerErrorInvalidDevice;
        case paDeviceUnavailable:
            return AudioManagerErrorDeviceUnavailable;
        default:
            return AudioManagerErrorCodeOther;
    }
}

void AudioManager::setAudioProcessingUnit(AudioProcessingUnit *unit)
{
    _pimpl->dspKernel->audioProcessingUnit = unit;
    if (unit == NULL) {
        APUGetLogger()->log(0, "APUnit is null!");
        return;
    }
    unit->setSampleRate(_pimpl->dspKernel->sampleRate);
}

bool AudioManager::initialize()
{
    return _pimpl->dspKernel->open(getOutputDevice());
}

bool AudioManager::destroy()
{
    if (_pimpl->dspKernel->close())
    {
        return true;
    }
    
    return false;
}

bool AudioManager::start()
{
    _pimpl->dspKernel->audioProcessingUnit->setupInitialState();
    return _pimpl->dspKernel->start();
}

bool AudioManager::stop()
{
    return _pimpl->dspKernel->stop();
}

bool AudioManager::setInputFile(AudioFile *file)
{
    if (file == NULL || file->mode() == AudioFileModeWriteOnly)
        return false;
    
    _pimpl->dspKernel->audioFile = file;
    
    return true;
}

AudioFile * AudioManager::getInputFile() const
{
    return _pimpl->dspKernel->audioFile;
}

AudioManagerStatus AudioManager::getStatus() const
{
    return _pimpl->dspKernel->status;
}

void AudioManager::sleep(unsigned long millisec)
{
    Pa_Sleep(millisec);
}

void AudioManager::setLooping(bool looping)
{
    _pimpl->dspKernel->audioFile->setLooping(looping);
}

void AudioManager::subscribe(APUHostEventSink *eventSink)
{
    if (eventSink) {
        _pimpl->eventSinks->addObject(eventSink);
    }
}

void AudioManager::unsubscribe(APUHostEventSink *eventSink)
{
    if (eventSink && _pimpl->eventSinks->size()) {
        _pimpl->eventSinks->removeObject(eventSink);
    }
}

void AudioManager::streamStatusChangeCallback(void *ctx)
{
    AudioManager *audioManager = (AudioManager *)ctx;
    if (audioManager->_pimpl->eventSinks->size()) {
        audioManager->_pimpl->eventSinks->reset();
        do {
            audioManager->_pimpl->eventSinks->getCurrent()->handleEvent(APUHOST_EVENT_STATUSCHANGED,
                                                                        audioManager);
        } while (audioManager->_pimpl->eventSinks->moveNext());
    }
}

AudioManager::pimpl::~pimpl()
{
    assert(eventSinks->size() == 0);
    delete dspKernel;
}
