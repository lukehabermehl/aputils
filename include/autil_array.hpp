//
//  autil_array.hpp
//  libaputils
//
//  Created by Luke on 6/13/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_array_h
#define autil_array_h

#include "autil_obj.hpp"
#include "autil_obj_enum.hpp"

template <class T>
class APUArray
: public APUEnumerable<T>
, private APUObject
{
private:
    size_t m_size;
    size_t m_currentIdx;
    APUPtr<T> *m_itemArray;
public:
    APUOBJ_FWDDECL

    APUArray()
    : m_size(0)
    , m_currentIdx(0)
    , m_itemArray(NULL)
    {}

    APUArray(APUEnumerable<T> *enumerable) {
        m_currentIdx = 0;
        enumerable->reset();
        m_size = enumerable->size();
        if (m_size == 0) {
            m_itemArray = NULL;
        } else {
            m_itemArray = new APUPtr<T>[m_size];
            if (enumerable->getCurrent()) {
                size_t idx = 0;
                do {
                    T *item = enumerable->getCurrent();
                    m_itemArray[idx++] = item;
                } while (enumerable->moveNext());
            }
        }
    }
    virtual ~APUArray() {
        if (m_itemArray) {
            delete[] m_itemArray;
        }
    }

    size_t size() {
        return m_size;
    }

    T * getAt(size_t idx) {
        if (idx >= m_size) {
            return NULL;
        }
        return m_itemArray[idx];
    }

    // APUEnumerable
    void reset() {
        m_currentIdx = 0;
    }

    T * getCurrent() {
        return getAt(m_currentIdx);
    }

    bool moveNext() {
        m_currentIdx++;
        return (m_currentIdx < m_size);
    }
};


#endif /* autil_array_h */
