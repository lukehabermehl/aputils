//
//  autil_obj_enum.hpp
//  libaputils
//
//  Created by Luke on 5/21/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_obj_enum_hpp
#define autil_obj_enum_hpp

#include "autil_obj.hpp"
#include <cassert>

/** Interface for enumerable collections. 
  * The class T must conform to APUObjectInterface
  */
template <class T>
class APUEnumerable
: public APUObjectInterface
{
public:
    /** Reset the position of the iterator to the beginning of the collection 
      * Always call this before iterating.
      */
    virtual void reset() = 0;
    /** Get the object at the current position
      * @return NULL if the the position is not valid
      */
    virtual APUObjRet<T> getCurrent() = 0;
    /** Advance the current iterator position
      * @return true if the position is valid after advancing. Else, false
      */
    virtual bool moveNext() = 0;
    /** Get the number of items in the collection */
    virtual size_t size() = 0;

    virtual ~APUEnumerable() {}
};

/** Mutable enumerable collection of APUObjects implemented as a linked list */
template <class T>
class APUEnumerator
: public APUEnumerable<T>
, private APUObject
{
    struct Item
    {
        APUPtr<T> current;
        Item *next;
    };

    Item *current;
    Item *first;
    Item *last;

    size_t m_size;

public:
    APUOBJ_FWDDECL

    APUEnumerator()
    : current(NULL)
    , first(NULL)
    , last(NULL)
    , m_size(0)
    {
    }

    ~APUEnumerator()
    {
        Item *item = first;
        while (item) {
            Item *temp = item;
            item = item->next;
            delete temp;
        }
    }

    /** Add an object to the end of the linked list 
      * @param obj non-null pointer to the object to add
      */
    void addObject(T * obj) {
        assert(obj != NULL);
        if (obj == NULL) {
            return;
        }
        Item *item = new Item();
        item->current = obj;
        item->next = NULL;

        if (last) {
            last->next = item;
            last = item;
        } else {
            first = item;
            last = item;
        }

        if (!current) {
            current = first;
        }

        m_size++;
    }

    /** Remove an object from the list (via linear search) */
    void removeObject(T * obj) {
        if (first->current.ptr() && obj) {
            if (obj == first->current.ptr()) {
                Item *temp = first;
                first = first->next;
                if (obj == last->current.ptr()) {
                    last = first;
                }
                delete temp;
                m_size--;
            } else {
                Item *current = first->next;
                Item *prev = first;
                while (current) {
                    if (current->current.ptr() == obj) {
                        prev->next = current->next;
                        delete current;
                        m_size--;
                        break;
                    }
                }
            }
        }
    }

    void reset() {
        current = first;
    }

    APUObjRet<T> getCurrent() {
        return current == NULL ? (T*)NULL : (T*)current->current;
    }

    bool moveNext() {
        current = current->next;
        return (current != NULL);
    }

    size_t size() {
        return m_size;
    }
};


#endif /* autil_obj_enum_hpp */
