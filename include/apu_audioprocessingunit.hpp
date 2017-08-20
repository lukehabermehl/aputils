//
//  libblockdsp
//
//  Created by Luke on 5/24/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file apu_audioprocessingunit.hpp */

#ifndef AudioProcessingUnit_hpp
#define AudioProcessingUnit_hpp

#include "apu_parameter.hpp"
#include "apu_parametermap.hpp"
#include <unordered_map>
#include <string>

/** Base class for APU process units */
class AudioProcessingUnit
: public APUObject
{
public:
    APUOBJ_FWDDECL

    AudioProcessingUnit();
    virtual ~AudioProcessingUnit();

    /** This method is called by the APU host before starting any processing. Configure variables and default values in an override */
    virtual void setupInitialState();

    /** Override this method to do audio processing */
    virtual void processAudio(float *inputBuffer, float *outputBuffer, int numInputChannels, int numOutputChannels);
    
    /** Set the maximum number of input channels usable */
    virtual void setMaxInputChannels(uint32_t num);
    /** Set the maximum number of output channels usable */
    virtual void setMaxOutputChannels(uint32_t num);
    
    /** Set the sample rate.
      * Subclasses MUST call on base class implementation, as it updates
      * the sample rate for registered parameters.
      */
    virtual void setSampleRate(unsigned long sampleRate);
    /** Get the sample rate */
    unsigned long getSampleRate();
    /** Get the name of the APU */
    virtual APUObjRet<APUString> getName();

    /** Override this to provide a fallback sample rate in the case
      * that the host has not specified one */
    virtual unsigned long getPreferredSampleRate();

    /** Get the number of parameters associated with this APU */
    size_t getNumParameters();
    /** Get the parameter with the given ID.
      * @return the parameter or NULL if not found
      */
    APUObjRet<APUParameter> getParameterWithIdentifier(const char *identifier);
    /** Get the parameter map */
    const APUParameterMap * getParameterMap();
    
    static APUObjRet<AudioProcessingUnit> createPassthroughUnit();
    
protected:
    /** Add a parameter to the APU 
      * @return true if the parameter was added successfully, false if a parameter with that name exists already
      */
    virtual bool addParameter(APUParameter *param);
    
private:
    class Pimpl;
    Pimpl *m_pimpl;
};

#endif /* AudioProcessingUnit_hpp */
