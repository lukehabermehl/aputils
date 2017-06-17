//
//  autil_str.hpp
//  libaputils
//
//  Created by Luke on 6/13/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_str_h
#define autil_str_h

#include "autil_obj.hpp"

class APUString
: public APUObject
{
public:
    APUOBJ_FWDDECL

    APUString();
    APUString(const char *cstr);
    virtual ~APUString();

    size_t length();

    const char *str();
    APUString *copy();
    APUString *append(APUString *str);

    static APUString *format(const char *fmt, ...);

    bool equals(APUString *);
    bool compare(APUString *);

private:
    class Pimpl;
    Pimpl *pimpl_;
};


#endif /* autil_str_h */
