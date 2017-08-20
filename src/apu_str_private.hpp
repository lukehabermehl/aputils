//
//  apu_str_private.hpp
//  libaputils
//
//  Created by Luke on 6/13/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_str_private_h
#define apu_str_private_h

#include "apu_str.hpp"
#include <string>

class APUString::Pimpl
{
public:
    Pimpl()
    : strbuf(NULL)
    , length(0)
    {}

    ~Pimpl() {
        if (strbuf) {
            delete[] strbuf;
        }
    }

    char *strbuf;
    size_t length;

    void setStr(const char *str) {
        size_t len = strlen(str);
        char *buf = new char[len + 1];
        buf[len] = '\0';
        strcpy(buf, str);

        if (strbuf) {
            delete[] strbuf;
        }
        strbuf = buf;
        length = len;
    }
};


#endif /* apu_str_private_h */
