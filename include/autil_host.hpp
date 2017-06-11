//
//  autil_host.hpp
//  libaputils
//
//  Created by Luke on 5/14/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_host_h
#define autil_host_h

#include "autil_obj.hpp"
#include "autil_obj_enum.hpp"

#define APUHOST_EVENT_STATUSCHANGED "status_changed"

/** Indicates where the Audio Manager will pull input from */
enum AudioInputMode
{
    AudioInputModeNone,
    AudioInputModeFile,
    AudioInputModeDevice
};

/** Use AudioDeviceIndex when referring to the integer index of an audio device */
typedef int AudioDeviceIndex;

/** Contains useful information about an input or output device */
class AudioDevice
: public APUObject
{
public:
    APUOBJ_FWDDECL

    AudioDevice(AudioDeviceIndex index, const char *name)
    : name_(name)
    , index_(index)
    {}

    virtual ~AudioDevice()
    {}

    const char *getName() const { return name_.c_str(); }
    AudioDeviceIndex getIndex() const { return index_; }

private:
    std::string name_;
    AudioDeviceIndex index_;
};

class APUHostInterface;

class APUHostEventSink
: public APUObjectInterface
{
public:
    virtual ~APUHostEventSink() {}
    virtual void handleEvent(const char *eventName, APUHostInterface *sender) = 0;
};


class APUHostInterface
: public APUObjectInterface
{
public:
    enum ErrorCode {
        OK = 0,
        UNINITIALIZED,
        INVALID_CHANNEL_COUNT,
        INVALID_SAMPLE_RATE,
        INVALID_DEVICE,
        DEVICE_UNAVAILABLE,
        UNKNOWN,
    };

    enum Status {
        DONE = 0,
        RUNNING
    };

    virtual void setInputMode(AudioInputMode inputMode) = 0;
    virtual APUEnumerable<AudioDevice> * getDevices() const = 0;
    virtual void setInputDevice(AudioDeviceIndex index) = 0;
    virtual AudioDeviceIndex getInputDevice() const = 0;
    virtual void setOutputDevice(AudioDeviceIndex index) = 0;
    virtual AudioDeviceIndex getOutputDevice() const = 0;

    virtual void setAudioProcessingUnit(AudioProcessingUnit *unit) = 0;
    virtual void setNumOutputChannels(int numOutputChannels) = 0;
    virtual int getNumOutputChannels() const = 0;

    virtual bool initialize() = 0;
    virtual bool destroy() = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;

    virtual Status getStatus() const = 0;
    virtual ErrorCode getError() const = 0;
    virtual void subscribe(APUHostEventSink *eventSink) = 0;
    virtual void unsubscribe(APUHostEventSink *eventSink) = 0;

    virtual bool setInputFile(AudioFile *file) = 0;
    virtual AudioFile *getInputFile() const = 0;
};

#endif /* autil_host_h */
