//
//  autil_str.cpp
//  libaputils
//
//  Created by Luke on 6/13/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include "autil_str.hpp"
#include "autil_str_private.hpp"

APUString::APUString()
{
    pimpl_ = new Pimpl();
    pimpl_->setStr("");
}

APUString::APUString(const char *cstr)
{
    pimpl_ = new Pimpl();
    pimpl_->setStr(cstr);
}

APUString::~APUString()
{
    delete pimpl_;
}

size_t
APUString::length()
{
    return pimpl_->length;
}

const char *
APUString::str()
{
    return pimpl_->strbuf;
}

APUObjRet<APUString>
APUString::copy()
{
    APUString *copystr = new APUString(str());
    return copystr;
}

APUObjRet<APUString>
APUString::append(APUString *appendStr)
{
    if (!appendStr) {
        return copy();
    }
    return APUString::format("%s%s", str(), appendStr->str());
}

bool
APUString::equals(APUString *rhs)
{
    if (!rhs) {
        return false;
    }

    return 0 == strcmp(str(), rhs->str());
}

bool
APUString::compare(APUString *rhs)
{
    if (!rhs) {
        return false;
    }
    int c = strcmp(str(), rhs->str());
    return c < 0;
}

APUObjRet<APUString>
APUString::format(const char *fmt, ...)
{
    if (fmt == NULL) {
        return new APUString();
    }
    va_list args;
    va_start(args, fmt);

    size_t bufSize = 4096;
    char *buf = new char[4096];
    int n = vsnprintf(buf, bufSize, fmt, args);
    if (n < 0) {
        return NULL;
    }
    else if (n >= bufSize) {
        bufSize = n+1;
        delete[] buf;
        buf = new char[bufSize];
        n = vsnprintf(buf, bufSize, fmt, args);
        if (n < 0 || n >= bufSize) {
            return NULL;
        }
    }

    APUString *ret = new APUString(buf);
    delete[] buf;
    return ret;
}
