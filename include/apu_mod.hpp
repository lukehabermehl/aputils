//
//  apu_mod.hpp
//  libaputils
//
//  Created by Luke on 7/24/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_mod_h
#define apu_mod_h

#include "apu_obj.hpp"

/** Interface for modulation sources */
class APUModSource
: public APUObjectInterface
{
public:
    /** Get the bipolar modulation value (-1.0 -- 1.0) */
    virtual float getModulationValue() = 0;
    /** Prepare for the next modulation value */
    virtual void next() = 0;

    virtual ~APUModSource() {}
};

#endif /* apu_mod_h */
