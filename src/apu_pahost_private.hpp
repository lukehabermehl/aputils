//
//  apu_pahost_private.hpp
//  libaputils
//
//  Created by Luke on 5/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef apu_pahost_private_h
#define apu_pahost_private_h

#include "apu_pahost.hpp"
#include "apu_obj_enum.hpp"
#include "portaudio_host.hpp"

class APUPortAudioHost::Pimpl
{
public:
    ~Pimpl();
    PortAudioKernel *dspKernel;
    AudioDeviceIndex outputDeviceIndex;
    AudioDeviceIndex inputDeviceIndex;
    APUPtr<APUEnumerator<APUHostEventSink> > eventSinks;
};

#endif /* apu_pahost_private_h */
