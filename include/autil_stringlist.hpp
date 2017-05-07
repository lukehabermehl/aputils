//
//  autil_stringlist.hpp
//  libblockdsp
//
//  Created by Luke on 9/8/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_stringlist_hpp
#define autil_stringlist_hpp

#include <memory>
#include "autil_obj.hpp"

class APUStringList
: public APUObject
{
public:
    APUOBJ_FWDDECL

    APUStringList();

    void append(const char *str);
    size_t size();
    const char *getString(size_t index);
    void setString(size_t index, const char *str);

    APUPtr<APUStringList> copy();

private:
    class Pimpl;
    Pimpl *pimpl_;
};

#endif /* autil_stringlist_hpp */
