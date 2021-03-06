//
//  libaputils
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file apu_pahost.hpp */

#ifndef APUPortAudioHost_hpp
#define APUPortAudioHost_hpp

#include "apu_audioprocessingunit.hpp"
#include "apu_file.hpp"
#include "apu_host.hpp"

/** Host implementation using PortAudio */
class APUPortAudioHost
: public APUHostInterface
, private APUObject
{
public:
    APUOBJ_FWDDECL

    APUPortAudioHost();
    virtual ~APUPortAudioHost();

    /** Indicates the type of input source to use */
    enum AudioInputMode
    {
        /** There is no input (synth mode) */
        INPUT_NONE,
        /** Use a file for input */
        INPUT_FILE,
        /** Use an audio device for input */
        INPUT_DEVICE
    };

    /** Specify the audio input source type */
    virtual void setInputMode(AudioInputMode mode);

    /** @return the current audio input source type */
    virtual AudioInputMode getInputMode();

    /** Get a list of the available audio devices */
    virtual APUObjRet<APUEnumerable<AudioDevice> > getDevices() const;

    /** Set the input device index (use with INPUT_DEVICE mode) */
    virtual void setInputDevice(AudioDeviceIndex devIndex);

    /** Get the current input device index.
     @returns -1 if no input is selected or if using file
     */
    virtual AudioDeviceIndex getInputDevice() const;

    /** Set the output device */
    virtual void setOutputDevice(AudioDeviceIndex devIndex);

    /** Get the current output device index */
    virtual AudioDeviceIndex getOutputDevice() const;

    /** Pass in an instance of AudioProcessingUnit to do the DSP */
    virtual void setAudioProcessingUnit(AudioProcessingUnit *unit);

    /** Set the number of output channels */
    virtual void setNumOutputChannels(int numOutputChannels);

    virtual int getNumOutputChannels() const;

    /** Open the audio I/O streams. Returns true if successful */
    virtual bool initialize();

    /** Close the audio I/O streams. Returns true if successful */
    virtual bool destroy();

    /** Start audio processing. Returns true if successful */
    virtual bool start();

    /** Stop audio processing. Returns false if successful */
    virtual bool stop();

    /** Get the current status of the Audio Manager */
    virtual APUHostInterface::Status getStatus() const;

    /** Get the error code of the last error the ocurred.
     * @return APUPortAudioHostErrorNoError is everything is OK
     */
    virtual APUHostInterface::ErrorCode getError() const;

    virtual void subscribe(APUHostEventSink *eventSink);
    virtual void unsubscribe(APUHostEventSink *eventSink);

    /** Load input from file */
    virtual bool setInputFile(AudioFile *file);

    /** Get the current input file.
      * @return NULL if none has been specified
      */
    virtual APUObjRet<AudioFile> getInputFile() const;

    /** Enable/disable looping when using an input file */
    virtual void setLooping(bool looping);

    /** Sleep the calling thread */
    virtual void sleep(unsigned long millisec);
    
private:
    class Pimpl;
    Pimpl *m_pimpl;
    
    static void streamStatusChangeCallback(void *ctx);
};


#endif /* APUPortAudioHost_hpp */
