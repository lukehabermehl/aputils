//
//  autil_parametermap.hpp
//  libblockdsp
//

/** @file autil_parametermap.hpp */

#ifndef autil_parametermap_hpp
#define autil_parametermap_hpp

#include "autil_parameter.hpp"
#include "autil_obj.hpp"
#include <memory>

/** @file autil_parametermap.hpp */

/** HashMap implementation that maps parameters by name.
 */
class APUParameterMap
: public APUObject
{
public:

    /** A simple iterator object that behaves similarly to std::iterator, but is not a pointer */
    class Iterator
    {
        friend class APUParameterMap;
        
    public:
        /** The name of the parameter */
        APUObjRet<APUString> first();
        /** The parameter instance */
        APUObjRet<APUParameter> second();
        /** Move the iterator forward to the next key/value pair */
        Iterator& operator++();
        /** @return true if iterator points to a valid key/value pair. Otherwise, false. */
        bool valid();

    private:
        class Pimpl;
        Pimpl * pimpl_;
    };

    APUOBJ_FWDDECL

    APUParameterMap();
    virtual ~APUParameterMap();

    void addParameter(APUParameter *parameter);
    APUObjRet<APUParameter> parameterWithName(const char *name) const;
    size_t size() const;

    Iterator begin() const;

private:
    class Pimpl;
    Pimpl *pimpl_;
};

#endif /* autil_parametermap_hpp */
