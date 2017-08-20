//
//  APUPortAudioHost_Private.hpp
//  libblockdsp
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef APUPortAudioHost_Private_h
#define APUPortAudioHost_Private_h

#include "apu_pahost.hpp"
#include "portaudio_host.hpp"
#include "apu_obj_enum.hpp"

#include <string.h>

class APUPortAudioHost::Pimpl
{
public:
    ~Pimpl();
    PortAudioKernel *dspKernel;
    AudioDeviceIndex outputDeviceIndex;
    AudioDeviceIndex inputDeviceIndex;
    APUPtr<APUEnumerator<APUHostEventSink> > eventSinks;
};

#endif /* APUPortAudioHost_Private_h */
