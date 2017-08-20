//
//  apu_file_private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef apu_file_private_h
#define apu_file_private_h

#include "apu_file.hpp"
#include <sndfile.h>
#include <thread>
#include <mutex>

class AudioFile::Pimpl
{
public:
    ~Pimpl();
    
    SNDFILE *sndfile;
    SF_INFO sfInfo;
    float *bufs[2];
    int currentBufIndex;
    size_t readIndex;
    size_t samplesRead;
    size_t framesBuffered;
    size_t totalSize;
    size_t bufferSize;
    bool looping;
    bool needsBuffer;
    bool isBuffering;
};

#endif /* apu_file_private_h */
