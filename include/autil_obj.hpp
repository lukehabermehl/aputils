//
//  autil_obj.hpp
//  libaputils
//
//  Created by Luke on 5/7/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_m_objh
#define autil_m_objh

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

    T *m_obj;
    void decRef()
    {
        if (m_obj && m_obj->decRef() == 0) {
            delete m_obj;
            m_obj = NULL;
        }
    }
public:
    APUPtr(T *obj=NULL)
    : m_obj(obj)
    {
        if (m_obj) {
            m_obj->addRef();
        }
    }

    APUPtr(const APUPtr<T> &orig)
    {
        m_obj = orig.m_obj;
        if (m_obj) {
            m_obj->addRef();
        }
    }

    APUPtr(const APUObjRet<T>& orig)
    {
        m_obj = orig.ptr();
        if (m_obj) {
            m_obj->addRef();
        }
    }

    T *operator->()
    {
        return m_obj;
    }

    bool operator==(const APUPtr<T> &rhs)
    {
        return m_obj == rhs.m_obj;
    }

    APUPtr<T>& operator=(const APUPtr<T> &rhs)
    {
        if (this == &rhs || m_obj == rhs.m_obj) {
            return *this;
        }

        if (m_obj) {
            decRef();
        }

        if (rhs.m_obj) {
            rhs.m_obj->addRef();
        }

        decRef();

        m_obj = rhs.m_obj;
        return *this;
    }

    APUPtr<T>& operator=(T * rhs)
    {
        if (m_obj == rhs) {
            return *this;
        }

        if (m_obj) {
            decRef();
        }

        if (rhs) {
            rhs->addRef();
        }

        m_obj = rhs;
        return *this;
    }

    operator T* ()
    {
        return m_obj;
    }

    operator bool() const
    {
        return m_obj != NULL;
    }

    operator APUObjRet<T>()
    {
        return APUObjRet<T>(m_obj);
    }

    ~APUPtr<T>()
    {
        decRef();
    }

    T * ptr()
    {
        return m_obj;
    }

    void clear()
    {
        if (m_obj) {
            decRef();
            m_obj = NULL;
        }
    }
};

template <class T>
class APUObjRet
{
    T * m_obj;
    bool m_hasRef;
public:
    APUObjRet(T *obj=NULL)
    : m_obj(obj)
    , m_hasRef(false)
    {
    }

    APUObjRet(const APUPtr<T>& rhs)
    {
        m_obj = rhs.m_obj;
        if (m_obj) {
            m_obj->addRef();
            m_hasRef = true;
        } else {
            m_hasRef = false;
        }
    }

    ~APUObjRet()
    {
        if (m_hasRef) {
            m_obj->decRef();
        }
        if (m_obj && m_obj->getRefCount() == 0) {
            delete m_obj;
        }
    }

    T *operator->()
    {
        return m_obj;
    }

    APUObjRet<T>& operator=(T * rhs)
    {
        if (m_obj == rhs) {
            return *this;
        }

        m_obj = rhs;
        return *this;
    }

    operator T* ()
    {
        return m_obj;
    }

    operator bool() const
    {
        return m_obj != NULL;
    }

    T * ptr() const
    {
        return m_obj;
    }
};


#endif /* autil_m_objh */
