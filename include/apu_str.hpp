//
//  apu_str.hpp
//  libaputils
//
//  Created by Luke on 6/13/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_str_h
#define apu_str_h

#include "apu_obj.hpp"

/** APUObject wrapper for a string */
class APUString
: public APUObject
{
public:
    APUOBJ_FWDDECL

    /** Construct an empty string */
    APUString();
    /** Construct a string with the given characters (null-terminated) */
    APUString(const char *cstr);
    virtual ~APUString();

    /** The number of ascii characters in the string */
    size_t length();

    /** Get a pointer to the internal null-terminated char buffer */
    const char *str();
    /** Create a deep copy of the string */
    APUObjRet<APUString> copy();
    /** Get a new string whose value is that of the target object appended with that of the argument
      * The target and the arguments are left unchanged.
      * @param str the string to append. If NULL, the behavior is equal to copy()
      * @return a new string instance
      */
    APUObjRet<APUString> append(APUString *str);

    /** Create a string with a C-style format and arguments.
      * Example:
      * @code
      *     APUPtr<APUString> formatted = APUString::format("%s, %s!", "Hello", "world");
      * @endcode
      */
    static APUObjRet<APUString> format(const char *fmt, ...);

    /** Equivalent to APUStringMake(). */
    static APUObjRet<APUString> newInstance(const char *str);

    /** Determine if a string has the same value as the target
      * @return true if the target and the argument have the same value
      */
    bool equals(APUString *);
    bool equals(const char *);

    /** Compare two strings alphabetically. Can be used with the likes of std::sort()
      * @return true if the target string is ordered before the argument, else return false.
      */
    bool compare(APUString *);
    bool compare(const char *);

protected:
    void * operator new(size_t);

private:
    class Pimpl;
    Pimpl *m_pimpl;
};

/** Helper to create a new APUString reference.
  * Use this function instead of calling the new() operator
  */
APUObjRet<APUString> APUStringMake(const char *str);

/** An object that wraps APUStringRef so that it can have overridden operators and behave like a normal C++ object. */
class APUStringRef
{
public:
    /** Get the underlying APUString */
    APUObjRet<APUString> get() const;

    /** Construct an empty string */
    APUStringRef();
    /** Wrap the given APUString instance. Will create a strong reference */
    APUStringRef(APUString *str);
    APUStringRef(const APUPtr<APUString>& str);
    APUStringRef(const APUObjRet<APUString>& str);

    /** Create a new APUString with then input buffer */
    APUStringRef(const char *str);

    APUStringRef& operator=(const char *rhs);
    APUStringRef& operator=(const APUStringRef& rhs);
    APUStringRef& operator=(APUString *rhs);

    /** Set the underlying string to be the result of the current string appended with the input string */
    void append(const APUStringRef& appendStr);

    /** Get the number of bytes in the string */
    size_t length() const;
    /** @return true if the string is zero bytes long */
    bool isEmpty() const;

    operator const char *() const;
    operator APUString *() const;

    bool operator==(const APUStringRef& rhs);
    bool operator==(const char *rhs);
    bool operator<(const APUStringRef& rhs);

private:
    APUPtr<APUString> m_string;
};

#endif /* apu_str_h */
