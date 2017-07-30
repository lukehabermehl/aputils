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

private:
    class Pimpl;
    Pimpl *pimpl_;
};


#endif /* autil_str_h */
