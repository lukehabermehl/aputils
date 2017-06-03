//
//  AudioManager_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef AudioManager_Private_h
#define AudioManager_Private_h

#include "autil_manager.hpp"
#include "portaudio_host.hpp"
#include "autil_obj_enum.hpp"

#include <string.h>

class AudioManager::pimpl
{
public:
    ~pimpl();
    PortAudioKernel *dspKernel;
    AudioDeviceIndex outputDeviceIndex;
    AudioDeviceIndex inputDeviceIndex;
    APUPtr<APUEnumerator<APUHostEventSink> > eventSinks;
};

#endif /* AudioManager_Private_h */
