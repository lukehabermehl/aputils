//
//  apu_parametermap_private.hpp
//  libblockdsp
//
//  Created by Luke on 9/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef apu_parametermap_private_h
#define apu_parametermap_private_h

#include "apu_parametermap.hpp"
#include <unordered_map>
#include <string>

class APUParameterMap::Pimpl
{
public:
    ~Pimpl();
    std::unordered_map<std::string, APUParameter *> params;
};

class APUParameterMap::Iterator::Pimpl
{
public:
    std::unordered_map<std::string, APUParameter *>::iterator mapIterator;
    std::unordered_map<std::string, APUParameter *> * mapPtr;
};


#endif /* apu_parametermap_private_h */
