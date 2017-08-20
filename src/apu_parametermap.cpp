//
//  apu_parametermap.cpp
//  libaputils
//
//  Created by Luke on 9/9/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "apu_parametermap.hpp"
#include "apu_parametermap_private.hpp"

APUParameterMap::APUParameterMap()
: APUObject()
{
    m_pimpl = new Pimpl();
}

APUParameterMap::~APUParameterMap()
{
    delete m_pimpl;
}

void APUParameterMap::addParameter(APUParameter *parameter)
{
    if (parameter == NULL)
        return;

    APUPtr<APUParameter> param = parameterWithIdentifier(parameter->getIdentifier()->str());
    if (param.ptr() == parameter) {
        return;
    } else if (param) {
        param->decRef();
    }
    m_pimpl->params[parameter->getIdentifier()->str()] = parameter;
    parameter->addRef();
}

APUObjRet<APUParameter> APUParameterMap::parameterWithIdentifier(const char *identifier) const
{
    auto it = m_pimpl->params.find(std::string(identifier));
    if (it == m_pimpl->params.end())
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
    itPimpl->mapPtr = &m_pimpl->params;
    itPimpl->mapIterator = m_pimpl->params.begin();

    Iterator it;
    it.m_pimpl = itPimpl;

    return it;
}

APUString * APUParameterMap::Iterator::first()
{
    if (!valid()) return NULL;
    return m_pimpl->mapIterator->second->getIdentifier();
}

APUParameter *APUParameterMap::Iterator::second()
{
    if (!valid()) return NULL;
    return m_pimpl->mapIterator->second;
}

size_t APUParameterMap::size() const
{
    return m_pimpl->params.size();
}

APUParameterMap::Iterator& APUParameterMap::Iterator::operator++()
{
    m_pimpl->mapIterator++;
    return (*this);
}

bool APUParameterMap::Iterator::valid()
{
    return m_pimpl->mapIterator != m_pimpl->mapPtr->end();
}
