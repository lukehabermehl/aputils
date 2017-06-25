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

/** An immutable array that holds strong references to its objects */
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

    /** Constrcut an empty array */
    APUArray()
    : m_size(0)
    , m_currentIdx(0)
    , m_itemArray(NULL)
    {}

    /** Construct an array from an enumerable. The constructed array will
      * contain all of the items in the enumerable in the order that they are
      * enumerated.
      */
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

    /** The number of items in the array */
    size_t size() {
        return m_size;
    }

    /** Get the item at the given index
      * @param idx the 0-based index
      * @return the item or NULL if the index is out of range
      */
    APUObjRet<T> getAt(size_t idx) {
        if (idx >= m_size) {
            return NULL;
        }
        return m_itemArray[idx];
    }

    // APUEnumerable
    void reset() {
        m_currentIdx = 0;
    }

    APUObjRet<T> getCurrent() {
        return getAt(m_currentIdx);
    }

    bool moveNext() {
        m_currentIdx++;
        return (m_currentIdx < m_size);
    }
};


#endif /* autil_array_h */
