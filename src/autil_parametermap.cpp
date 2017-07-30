//
//  autil_parametermap.cpp
//  libblockdsp
//
//  Created by Luke on 9/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_parametermap.hpp"
#include "autil_parametermap_private.hpp"

APUParameterMap::APUParameterMap()
: APUObject()
{
    pimpl_ = new Pimpl();
}

APUParameterMap::~APUParameterMap()
{
    delete pimpl_;
}

void APUParameterMap::addParameter(APUParameter *parameter)
{
    if (parameter == NULL)
        return;

    APUPtr<APUParameter> param = parameterWithName(parameter->getName()->str());
    if (param.ptr() == parameter) {
        return;
    } else if (param) {
        param->decRef();
    }
    pimpl_->params[parameter->getName()->str()] = parameter;
    parameter->addRef();
}

APUParameter * APUParameterMap::parameterWithName(const char *name) const
{
    auto it = pimpl_->params.find(std::string(name));
    if (it == pimpl_->params.end())
    {
        return NULL;
    }

    return it->second;
}

APUParameterMap::Pimpl::~Pimpl()
{
    for (auto it = params.begin(); it != params.end(); it++)
    {
        APUPtr<APUParameter> param = it->second;
        param->decRef();
    }
}

APUParameterMap::Iterator APUParameterMap::begin() const
{
    Iterator::Pimpl *itPimpl = new Iterator::Pimpl();
    itPimpl->mapPtr = &pimpl_->params;
    itPimpl->mapIterator = pimpl_->params.begin();

    Iterator it;
    it.pimpl_ = itPimpl;

    return it;
}

const char * APUParameterMap::Iterator::first()
{
    if (!valid()) return NULL;
    return pimpl_->mapIterator->first.c_str();
}

APUParameter * APUParameterMap::Iterator::second()
{
    if (!valid()) return NULL;
    return pimpl_->mapIterator->second;
}

size_t APUParameterMap::size() const
{
    return pimpl_->params.size();
}

APUParameterMap::Iterator& APUParameterMap::Iterator::operator++()
{
    pimpl_->mapIterator++;
    return (*this);
}

bool APUParameterMap::Iterator::valid()
{
    return pimpl_->mapIterator != pimpl_->mapPtr->end();
}

