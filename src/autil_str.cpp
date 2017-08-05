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
    APUString *copystr = APUStringMake(str());
    return copystr;
}

APUObjRet<APUString>
APUString::append(APUString *appendStr)
{
    if (!appendStr) {
        return copy();
    }

    char *buffer = new char[length() + appendStr->length() + 1];
    memcpy(buffer, pimpl_->strbuf, length());
    memcpy(&buffer[length()], appendStr->str(), appendStr->length());
    buffer[length() + appendStr->length()] = '\0';

    APUString *ret = ::new APUString(buffer);
    delete []buffer;

    return ret;
}

bool
APUString::equals(APUString *rhs)
{
    if (!rhs) return false;

    return 0 == strcmp(str(), rhs->str());
}

bool
APUString::equals(const char *rhs)
{
    if (!rhs) return false;

    return 0 == strcmp(str(), rhs);
}

bool
APUString::compare(const char *rhs)
{
    if (!rhs) return false;

    int c = strcmp(str(), rhs);
    return c < 0;
}

bool
APUString::compare(APUString *rhs)
{
    if (!rhs) return false;

    int c = strcmp(str(), rhs->str());
    return c < 0;
}

APUObjRet<APUString>
APUString::format(const char *fmt, ...)
{
    if (fmt == NULL) {
        return ::new APUString();
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

    APUString *ret = ::new APUString(buf);
    delete[] buf;
    return ret;
}

APUObjRet<APUString>
APUString::newInstance(const char *str)
{
    return ::new APUString(str);
}

APUObjRet<APUString>
APUStringMake(const char *str)
{
    return APUString::newInstance(str);
}

