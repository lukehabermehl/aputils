//
//  autil_obj.hpp
//  libaputils
//
//  Created by Luke on 5/7/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_obj_h
#define autil_obj_h

#include <cstdlib>

/// Use this macro to implement call forwarding of APUObjectInterface methods
/// to an APUObject base class. Add under the public: section of your subclass header
#define APUOBJ_FWDDECL virtual int addRef() { return APUObject::addRef(); } \
virtual int decRef() { return APUObject::decRef(); } \
virtual int getRefCount() { return APUObject::getRefCount(); }

/// Interface declaring reference counting methods for use with APUPtr
class APUObjectInterface
{
public:
    virtual int addRef() = 0;
    virtual int decRef() = 0;
    virtual int getRefCount() = 0;
};

/// Inheritable implementation of the reference counting methods above
/// When subclassing, use the APUOBJ_FWDDECL macro
class APUObject
: public APUObjectInterface
{
public:
    APUObject();
    virtual ~APUObject();

    int addRef();
    int decRef();
    int getRefCount();

private:
    int refCount_;
};

template <class T>
class APUObjRet;

/// A custom ptr-type class to facilitate reference counting
/// Can be used with classes that conform to APUObjectInterface
template <class T>
class APUPtr
{
    friend class APUObjRet<T>;

    T *obj_;
    void decRef()
    {
        if (obj_ && obj_->decRef() == 0) {
            delete obj_;
            obj_ = NULL;
        }
    }
public:
    APUPtr(T *obj=NULL)
    : obj_(obj)
    {
        if (obj_) {
            obj_->addRef();
        }
    }

    APUPtr(const APUPtr<T> &orig)
    {
        obj_ = orig.obj_;
        if (obj_) {
            obj_->addRef();
        }
    }

    APUPtr(const APUObjRet<T>& orig)
    {
        obj_ = orig.ptr();
        if (obj_) {
            obj_->addRef();
        }
    }

    T *operator->()
    {
        return obj_;
    }

    bool operator==(const APUPtr<T> &rhs)
    {
        return obj_ == rhs.obj_;
    }

    APUPtr<T>& operator=(const APUPtr<T> &rhs)
    {
        if (this == &rhs) {
            return *this;
        }

        if (rhs.obj_) {
            rhs.obj_->addRef();
        }

        decRef();

        obj_ = rhs.obj_;
        return *this;
    }

    APUPtr<T>& operator=(T * rhs)
    {
        if (obj_ == rhs) {
            return *this;
        }

        if (rhs) {
            rhs->addRef();
        }

        obj_ = rhs;
        return *this;
    }

    operator T* ()
    {
        return obj_;
    }

    operator bool()
    {
        return obj_ != NULL;
    }

    operator APUObjRet<T>()
    {
        return APUObjRet<T>(obj_);
    }

    ~APUPtr<T>()
    {
        decRef();
    }

    T * ptr()
    {
        return obj_;
    }

    void clear()
    {
        if (obj_) {
            decRef();
            obj_ = NULL;
        }
    }
};

template <class T>
class APUObjRet
{
    T * obj_;
    bool hasRef_;
public:
    APUObjRet(T *obj=NULL)
    : obj_(obj)
    , hasRef_(false)
    {
    }

    APUObjRet(const APUPtr<T>& rhs)
    {
        obj_ = rhs.obj_;
        if (obj_) {
            obj_->addRef();
            hasRef_ = true;
        } else {
            hasRef_ = false;
        }
    }

    ~APUObjRet()
    {
        if (hasRef_) {
            obj_->decRef();
        }
        if (obj_ && obj_->getRefCount() == 0) {
            delete obj_;
        }
    }

    T *operator->()
    {
        return obj_;
    }

    APUObjRet<T>& operator=(T * rhs)
    {
        if (obj_ == rhs) {
            return *this;
        }

        obj_ = rhs;
        return *this;
    }

    operator T* ()
    {
        return obj_;
    }

    operator bool() const
    {
        return obj_ != NULL;
    }

    T * ptr() const
    {
        return obj_;
    }
};


#endif /* autil_obj_h */
