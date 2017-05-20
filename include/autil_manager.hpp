//
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file autil_manager.hpp */

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include "autil_audioprocessingunit.hpp"
#include "autil_file.hpp"
#include "autil_host.hpp"
#include <memory>


class AudioManager;

/** Abstract callback class */
class AudioManagerStatusChangedCallback
{
public:
    /** Called when the audio manager's status changes.
      * @param audioManager a pointer to the audioManager instance that sent the notification */
    virtual void onStatusChanged(AudioManager *audioManager) = 0;
};

/** Provides an interface for loading and processing audio from a file or input device */
class AudioManager
: public APUHostInterface
, public APUObject
{
public:
    APUOBJ_FWDDECL

    AudioManager();
    virtual ~AudioManager();

    /** Specify the audio input source */
    virtual void setInputMode(AudioInputMode mode);

    /** Get a list of the available audio devices
     @returns a linked list of AudioDeviceInfo
     */
    virtual AudioDeviceInfoRef getDevices();

    /** Set the input device index (use with AudioInputModeDevice) */
    virtual void setInputDevice(AudioDeviceIndex devIndex);

    /** Get the current input device index.
     @returns -1 if no input is selected or if using file
     */
    virtual AudioDeviceIndex getInputDevice();

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
    virtual AudioManagerStatus getStatus() const;

    /** Get the error code of the last error the ocurred.
     * @return AudioManagerErrorNoError is everything is OK
     */
    virtual AudioManagerErrorCode getError() const;

    virtual void subscribe(APUHostEventSink *eventSink);
    virtual void unsubscribe(APUHostEventSink *eventSink);

    /** Load input from file */
    virtual bool setInputFile(AudioFile *file);

    /** Get the current input file.
      * @return NULL if none has been specified
      */
    virtual AudioFile *getInputFile() const;

    /** Enable/disable looping when using an input file */
    virtual void setLooping(bool looping);

    /** Sleep the calling thread */
    virtual void sleep(unsigned long millisec);
    
private:
    class pimpl;
    pimpl *_pimpl;
    
    static void streamStatusChangeCallback(void *ctx);
};


#endif /* AudioManager_hpp */
