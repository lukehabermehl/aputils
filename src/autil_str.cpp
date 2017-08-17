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
    m_pimpl = new Pimpl();
    m_pimpl->setStr("");
}

APUString::APUString(const char *cstr)
{
    m_pimpl = new Pimpl();
    m_pimpl->setStr(cstr);
}

APUString::~APUString()
{
    delete m_pimpl;
}

size_t
APUString::length()
{
    return m_pimpl->length;
}

const char *
APUString::str()
{
    return m_pimpl->strbuf;
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
    memcpy(buffer, m_pimpl->strbuf, length());
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

APUStringRef::APUStringRef()
{
    m_string = (APUString *)APUStringMake("");
}

APUStringRef::APUStringRef(APUString *str)
{
    m_string = str;
    if (!m_string) {
        m_string = (APUString *)APUStringMake("");
    }
}

APUStringRef::APUStringRef(const char *str)
{
    m_string = (APUString *)APUStringMake(str);
}

APUObjRet<APUString>
APUStringRef::get() const
{
    return m_string;
}

APUStringRef::operator const char * () const
{
    return m_string->str();
}

APUStringRef::operator APUString *() const
{
    return m_string.ptr();
}

bool
APUStringRef::operator==(const APUStringRef &rhs)
{
    return m_string->equals(rhs.get());
}

bool
APUStringRef::operator==(const char *rhs)
{
    return 0 == strcmp(m_string->str(), rhs);
}

bool
APUStringRef::operator<(const APUStringRef &rhs)
{
    return m_string->compare(rhs.get());
}

APUStringRef&
APUStringRef::operator=(const char *rhs)
{
    m_string = (APUString *)APUStringMake(rhs);
    return *this;
}

APUStringRef&
APUStringRef::operator=(const APUStringRef &rhs)
{
    m_string = (APUString *)rhs.get();
    return *this;
}

void
APUStringRef::append(const APUStringRef &appendStr)
{
    m_string = (APUString *)m_string->append(appendStr.get());
}

size_t
APUStringRef::length() const
{
    return m_string->length();
}

bool
APUStringRef::isEmpty() const
{
    return m_string->length() == 0;
}

