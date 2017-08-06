//
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

/** @file autil_file.hpp */

#ifndef AudioFile_hpp
#define AudioFile_hpp

#include <stdlib.h>
#include "autil_obj.hpp"

/** Modes for audio file I/O */
enum AudioFileMode
{
    AudioFileModeReadOnly,
    AudioFileModeWriteOnly,
    AudioFileModeReadWrite
};

/** Status codes for reading from audio files */
enum AudioFileBufferStatus
{
    AudioFileBufferStatusOK = 0,
    AudioFileBufferStatusDoneReading,
    AudioFileBufferStatusOutOfBounds
};

/** Allows buffering samples from files and supports a wide variety of audio file formats (Thank you libsndfile) */
class AudioFile
: public APUObject
{
public:
    APUOBJ_FWDDECL

    /** Constructor
      * @param filepath the path of the audio file
      * @param mode the I/O mode to open the file in
      */
    AudioFile(const char *filepath, AudioFileMode mode);
    ~AudioFile();
    
    /** The sample rate of the file */
    unsigned long sampleRate();
    
    /** The number of frames the file contains */
    unsigned long numFrames();
    
    /** The number of audio channels in the file */
    int numChannels();
    
    /** The total number of samples in the file */
    size_t totalSize();
    
    /** The IO mode the file was opened with */
    AudioFileMode mode();
    
    /** Set frame to point to the next audio frame
      * @param frame a double pointer to the first sample in the frame
      * @returns The status of the buffer after reading the frame
     */
    AudioFileBufferStatus nextFrame(float **frame);
    
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

#endif /* AudioFile_hpp */
