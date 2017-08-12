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
    /** There is no input (synth mode) */
    INPUT_NONE,
    /** Use a file for input */
    INPUT_FILE,
    /** Use an audio device for input */
    INPUT_DEVICE
};

/** Use AudioDeviceIndex when referring to the integer index of an audio device */
typedef int AudioDeviceIndex;

/** Contains useful information about an input or output device */
class AudioDevice
: public APUObject
{
public:
    APUOBJ_FWDDECL

    /** Host implementations can use this constructor to create device descriptions
      * @param index the unique index of the device
      * @param deviceName the name of the device
      */
    AudioDevice(AudioDeviceIndex index, APUString *deviceName)
    : m_name(deviceName)
    , m_index(index)
    {}

    virtual ~AudioDevice()
    {}

    /** Get the device name */
    APUObjRet<APUString> getName() const { return m_name; }
    /** Get the unique index of the device */
    AudioDeviceIndex getIndex() const { return m_index; }

private:
    APUPtr<APUString> m_name;
    AudioDeviceIndex m_index;
};

class APUHostInterface;

/** Interface for objects subscribing to host events */
class APUHostEventSink
: public APUObjectInterface
{
public:
    virtual ~APUHostEventSink() {}
    /** Called by the host on it's subscribers when an event occurs
      * @param eventName the name of the event
      * @param sender the host that is sending the event
      */
    virtual void handleEvent(APUString *eventName, APUHostInterface *sender) = 0;
};


/** Interface that hosts must implement */
class APUHostInterface
: public APUObjectInterface
{
public:
    /** Possible error codes for host operations */
    enum ErrorCode {
        OK = 0,
        UNINITIALIZED,
        INVALID_CHANNEL_COUNT,
        INVALID_SAMPLE_RATE,
        INVALID_DEVICE,
        DEVICE_UNAVAILABLE,
        UNKNOWN,
    };

    /** Possible host status codes */
    enum Status {
        DONE = 0,
        RUNNING
    };

    /** Set the input mode of the host */
    virtual void setInputMode(AudioInputMode inputMode) = 0;
    /** Get an enumerable list of all the available audio devices */
    virtual APUObjRet<APUEnumerable<AudioDevice> > getDevices() const = 0;
    /** Set the input audio device by index */
    virtual void setInputDevice(AudioDeviceIndex index) = 0;
    /** Get the index of the current input device */
    virtual AudioDeviceIndex getInputDevice() const = 0;
    /** Set the output audio device by index */
    virtual void setOutputDevice(AudioDeviceIndex index) = 0;
    /** Get the index of the current output device */
    virtual AudioDeviceIndex getOutputDevice() const = 0;

    /** Set the current processing unit. If NULL, the host should resort to a passthrough */
    virtual void setAudioProcessingUnit(AudioProcessingUnit *unit) = 0;
    /** Set the number of output channels to use */
    virtual void setNumOutputChannels(int numOutputChannels) = 0;
    /** Get the number of output channels */
    virtual int getNumOutputChannels() const = 0;

    /** Initialize the host. Implementations should allocate and initialize any
      * non-static objects.
      * @return true if initialization is successful, else false.
      */
    virtual bool initialize() = 0;
    /** Prepare the host for destruction. Implementations should deallocate memory. 
      * that was allocated in initialize().
      * @return true if destruction is successful, else false.
      */
    virtual bool destroy() = 0;
    /** Begin the processing loop
      * @return true if successful, else false
      */
    virtual bool start() = 0;
    /** Stop the processing loop.
      * @return true if successful, else false
      */
    virtual bool stop() = 0;

    /** Get the current state of the host */
    virtual Status getStatus() const = 0;
    /** Get the error code of the last attempted operation.
      * Callers should be able to idenfity why one of the above methods
      * returned false
      */
    virtual ErrorCode getError() const = 0;
    /** Subscribe an event sink.
      * @param eventSink an object that will receive events from the host
      */
    virtual void subscribe(APUHostEventSink *eventSink) = 0;
    /** Unsubscribe an event sink.
      * Note: the host will hold a strong reference to the event sink, so unsubscribes
      * MUST occur before the object can be destroyed.
      */
    virtual void unsubscribe(APUHostEventSink *eventSink) = 0;

    /** Set the file to use for audio input. The input mode must also be set to
      * AudioInputModeFile.
      */
    virtual bool setInputFile(AudioFile *file) = 0;

    /** Get a pointer to the current input file */
    virtual APUObjRet<AudioFile> getInputFile() const = 0;
};

#endif /* autil_host_h */
