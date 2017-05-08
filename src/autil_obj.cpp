//
//  autil_obj.cpp
//  libaputils
//
//  Created by Luke on 5/7/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include "autil_obj.hpp"

APUObject::APUObject()
{
    refCount_ = 0;
}

APUObject::~APUObject()
{
}

int APUObject::addRef()
{
    return ++refCount_;
}

int APUObject::decRef()
{
    return --refCount_;
}