//
//  apu_loader.h
//  libaputils
//
//  Created by Luke on 9/3/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_loader_h
#define apu_loader_h

#include "apu_audioprocessingunit.hpp"

/** Object for dynamically loading APU units from disk */
class APULoader
: public APUObject
{
public:
    APUOBJ_FWDDECL

    /** Possible error codes for loading units */
    enum ErrorCode {
        SUCCESS = 0,
        FILE_NOT_FOUND,
        LOAD_FAILURE,
        INVALID_UNIT
    };

    APULoader();
    virtual ~APULoader();

    /** Attempt to load a unit from the given file path.
      * @return true if a unit is successfully loaded, otherwise return false.
      * Call getError() to get the error if this method returns false.
      */
    virtual bool loadUnit(APUString *filepath);
    /** @return true if a unit is loaded */
    virtual bool hasUnit();
    /** @return the loaded unit, or NULL if no unit is loaded. */
    virtual APUObjRet<AudioProcessingUnit> getUnit();
    /** @return the code for the last error that occurred, or SUCCESS if everything is OK. */
    virtual ErrorCode getError();

private:
    APUPtr<AudioProcessingUnit> m_unit;
    ErrorCode m_error;
    void *m_dllHandle;
};


#endif /* apu_loader_h */
