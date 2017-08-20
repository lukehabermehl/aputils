//
//  apu_parametermap.hpp
//  libblockdsp
//

/** @file apu_parametermap.hpp */

#ifndef apu_parametermap_hpp
#define apu_parametermap_hpp

#include "apu_parameter.hpp"
#include "apu_obj.hpp"
#include <memory>

/** @file apu_parametermap.hpp */

/** HashMap implementation that maps parameters by name.
 */
class APUParameterMap
: public APUObject
{
public:

    /** A simple iterator object that behaves similarly to std::iterator */
    class Iterator
    {
        friend class APUParameterMap;
        
    public:
        /** The identifier of the parameter */
        APUString *first();
        /** The parameter instance */
        APUParameter *second();
        /** Move the iterator forward to the next key/value pair */
        Iterator& operator++();
        /** @return true if iterator points to a valid key/value pair. Otherwise, false. */
        bool valid();

    private:
        class Pimpl;
        Pimpl * m_pimpl;
    };

    APUOBJ_FWDDECL

    APUParameterMap();
    virtual ~APUParameterMap();

    /** Add a parameter to the map. The map holds a strong reference */
    void addParameter(APUParameter *parameter);
    /** Fetch the parameter with the given identifier from the map
      * @return NULL if no parameter matches that name
      */
    APUObjRet<APUParameter> parameterWithIdentifier(const char *identifier) const;

    /** Get the number of parameters in the map */
    size_t size() const;

    /** Get an iterator pointing to the first key/value pair in the map */
    Iterator begin() const;

private:
    class Pimpl;
    Pimpl *m_pimpl;
};

#endif /* apu_parametermap_hpp */
