//
//  libaputils
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file apu_file.hpp */

#ifndef apu_file_hpp
#define apu_file_hpp

#include "apu_obj.hpp"

/** Allows buffering samples from files and supports a wide variety of audio file formats (Thank you libsndfile) */
class AudioFile
: public APUObject
{
public:
    APUOBJ_FWDDECL

    /** Status codes for reading from audio files */
    enum BufferStatus
    {
        STATUS_OK = 0,
        DONE_READING,
        OUT_OF_BOUNDS
    };

    /** Constructor
      * @param filepath the path of the audio file
      */
    AudioFile(const char *filepath);
    ~AudioFile();
    
    /** The sample rate of the file */
    size_t sampleRate();
    
    /** The number of frames the file contains */
    size_t numFrames();
    
    /** The number of audio channels in the file */
    size_t numChannels();
    
    /** The total number of samples in the file */
    size_t totalSize();

    /** Set frame to point to the next audio frame
      * @param frame a double pointer to the first sample in the frame
      * @returns The status of the buffer after reading the frame
     */
    BufferStatus nextFrame(float **frame);
    
    /** Close the file */
    void close();
    /** Set looping enabled/disabled */
    void setLooping(bool looping);
    /** Determine if the audio file is set to loop */
    bool isLooping();
    
    class Pimpl;
private:
    Pimpl *m_pimpl;
};

#endif /* apu_file_hpp */
