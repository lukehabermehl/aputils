//
//  AudioFile.cpp
//  libblockdsp
//
//  Created by Luke on 5/23/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_file.hpp"
#include "autil_file_private.hpp"
#include "autil_thread_manager.hpp"
#include "autil_logger.hpp"

#include <vector>

#define FRAMES_PER_FILE_BUFFER 5000

static const char * kAudioFileLoggerPrefix = "[AudioFile]";

void file_buffer_worker(void *ctx)
{
    AudioFile::Pimpl *fileImpl = (AudioFile::Pimpl *)ctx;
    int writeBuffer = (fileImpl->currentBufIndex == 0) ? 1 : 0;
    sf_seek(fileImpl->sndfile, fileImpl->framesBuffered, SF_SEEK_SET);
    size_t read = sf_readf_float(fileImpl->sndfile, fileImpl->bufs[writeBuffer], FRAMES_PER_FILE_BUFFER);
    fileImpl->framesBuffered += read;
    if (read < FRAMES_PER_FILE_BUFFER)
    {
        if (fileImpl->looping)
        {
            sf_seek(fileImpl->sndfile, 0, SF_SEEK_SET);
            size_t samplesDidRead = read * fileImpl->sfInfo.channels;
            size_t framesToRead = (FRAMES_PER_FILE_BUFFER - read);
            sf_readf_float(fileImpl->sndfile, &(fileImpl->bufs[writeBuffer][samplesDidRead]), framesToRead);
            fileImpl->framesBuffered = framesToRead;
        }
        else
        {
            fileImpl->needsBuffer = false;
        }
    }
    
    fileImpl->isBuffering = false;
}

AudioFile::AudioFile(const char *filepath)
{
    int sfmode = SFM_READ;
    m_pimpl = new Pimpl;

    m_pimpl->sndfile = sf_open(filepath, sfmode, &m_pimpl->sfInfo);
    if (!m_pimpl->sndfile) {
        const char *errormsg = sf_strerror(m_pimpl->sndfile);
        APUGetLogger()->log(kAudioFileLoggerPrefix, LOG_LEVEL_ERROR,
                            "Failed to load file: %s",
                            errormsg);
        return;
    }
    m_pimpl->totalSize = m_pimpl->sfInfo.channels * m_pimpl->sfInfo.frames;
    m_pimpl->readIndex = 0;
    m_pimpl->currentBufIndex = 0;
    m_pimpl->samplesRead = 0;
    
    size_t framesPerBuffer = FRAMES_PER_FILE_BUFFER;
    if (framesPerBuffer > numFrames())
    {
        framesPerBuffer = numFrames();
        m_pimpl->bufferSize = framesPerBuffer * numChannels();
        m_pimpl->needsBuffer = false;
        m_pimpl->bufs[0] = new float[m_pimpl->bufferSize];
        m_pimpl->bufs[1] = NULL;
    }
    else
    {
        m_pimpl->bufferSize = framesPerBuffer * numChannels();
        m_pimpl->bufs[0] = new float[m_pimpl->bufferSize];
        m_pimpl->bufs[1] = new float[m_pimpl->bufferSize];
        m_pimpl->needsBuffer = true;
    }
    sf_seek(m_pimpl->sndfile, 0, SF_SEEK_SET);
    m_pimpl->framesBuffered = sf_read_float(m_pimpl->sndfile, &(m_pimpl->bufs[0][0]), m_pimpl->bufferSize) / numChannels();
    sf_seek(m_pimpl->sndfile, m_pimpl->framesBuffered, SF_SEEK_SET);
    APUDispatchThread(file_buffer_worker, m_pimpl);
}

AudioFile::~AudioFile()
{
    close();
    delete m_pimpl;
}

void AudioFile::close()
{
    if (m_pimpl->sndfile)
        sf_close(m_pimpl->sndfile);
    
    m_pimpl->sndfile = 0;
    
    if (m_pimpl->bufs[0]) {
        delete [] m_pimpl->bufs[0];
        m_pimpl->bufs[0] = NULL;
    }
    if (m_pimpl->bufs[1]) {
        delete [] m_pimpl->bufs[1];
        m_pimpl->bufs[1] = 0;
    }
}

AudioFile::BufferStatus
AudioFile::nextFrame(float **frame)
{
    if (m_pimpl->readIndex >= m_pimpl->bufferSize)
        return OUT_OF_BOUNDS;
    
    *frame = &((m_pimpl->bufs[m_pimpl->currentBufIndex])[m_pimpl->readIndex]);
    m_pimpl->readIndex += m_pimpl->sfInfo.channels;
    m_pimpl->samplesRead += m_pimpl->sfInfo.channels;
    
    if (m_pimpl->readIndex >= m_pimpl->bufferSize)
    {
        if (m_pimpl->currentBufIndex == 0)
        {
            m_pimpl->currentBufIndex = 1;
        }
        else
        {
            m_pimpl->currentBufIndex = 0;
        }
        m_pimpl->readIndex = 0;
        
        if (m_pimpl->needsBuffer && !m_pimpl->isBuffering)
        {
            m_pimpl->isBuffering = true;
            APUDispatchThread(file_buffer_worker, m_pimpl);
        }
    }
    
    if (m_pimpl->samplesRead >= totalSize())
    {
        if (isLooping())
        {
            m_pimpl->samplesRead = 0;
        }
        else
        {
            return DONE_READING;
        }
    }

    return STATUS_OK;
}

size_t
AudioFile::sampleRate()
{
    return m_pimpl->sfInfo.samplerate;
}

size_t
AudioFile::numFrames()
{
    return m_pimpl->sfInfo.frames;
}

size_t
AudioFile::numChannels()
{
    return m_pimpl->sfInfo.channels;
}

size_t AudioFile::totalSize()
{
    if (m_pimpl->totalSize == 0)
        m_pimpl->totalSize = m_pimpl->sfInfo.channels * m_pimpl->sfInfo.frames;
    
    return m_pimpl->totalSize;
}

void AudioFile::setLooping(bool looping)
{
    m_pimpl->looping = looping;
}

bool AudioFile::isLooping()
{
    return m_pimpl->looping;
}

AudioFile::Pimpl::~Pimpl()
{
    if (sndfile)
        sf_close(sndfile);
}
