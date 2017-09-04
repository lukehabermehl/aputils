//
//  apu_loader.cpp
//  libaputils
//
//  Created by Luke on 9/3/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include "apu_loader.hpp"

#include <dlfcn.h>

#define kAPUFactoryFuncName "AudioProcessingUnitFactoryCreate"

APULoader::APULoader()
: m_error(SUCCESS)
, m_dllHandle(NULL)
{
}

APULoader::~APULoader()
{
    if (m_dllHandle) {
        dlclose(m_dllHandle);
    }
}

bool
APULoader::hasUnit()
{
    return m_unit;
}

APUObjRet<AudioProcessingUnit>
APULoader::getUnit()
{
    return m_unit;
}

APULoader::ErrorCode
APULoader::getError()
{
    return m_error;
}

bool
APULoader::loadUnit(APUString *filepath)
{
    if (m_dllHandle) {
        dlclose(m_dllHandle);
        m_dllHandle = NULL;
        m_unit.clear();
    }

    assert(filepath);
    assert(filepath->length());

    if (!filepath || !filepath->length()) {
        m_error = FILE_NOT_FOUND;
        return false;
    }

    void *dllHandle = dlopen(filepath->str(), RTLD_NOW);
    if (!dllHandle) {
        m_error = LOAD_FAILURE;
        return false;
    }

    dlerror(); //clear dlerror code
    AudioProcessingUnit * (*apuFactoryFn)(void) = (AudioProcessingUnit *(*)(void))dlsym(dllHandle, kAPUFactoryFuncName);

    if (dlerror()) {
        m_error = INVALID_UNIT;
        return false;
    }

    m_unit = apuFactoryFn();
    if (!m_unit) {
        m_error = INVALID_UNIT;
        dlclose(dllHandle);
        return false;
    }

    m_dllHandle = dllHandle;
    m_error = SUCCESS;
    return true;
}
