//
//  autil_objmap.hpp
//  libaputils
//
//  Created by Luke on 5/13/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_objmap_h
#define autil_objmap_h

#include <unordered_map>
#include "autil_obj.hpp"

/** @file autil_objmap.hpp */

/** Hash map that holds strong references to its values. Values must conform to APUObjectInterface */
template <class K, class V>
class APUObjectMap
: public APUObject
{
    typedef std::unordered_map<K, V*> maptype;
    maptype m_map;

public:
    APUOBJ_FWDDECL

    /** Get the value for the given key
      * @return a weak pointer to the value or NULL if there is no value for the key
      */
    APUObjRet<V> get(const K& key) const
    {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            return it->second;
        }
        return NULL;
    }

    /** Insert the given key/value pair into the map.
      * @param key the hashable key for the value
      * @param value pointer to the value object. The ref count will be incremented
      */
    void put(const K& key, V *value)
    {
        V * existing = get(key);
        if (existing) {
            APUPtr<V> ref = existing;
            existing->decRef();
        }
        if (value) {
            value->addRef();
            m_map[key] = value;
        } else {
            m_map.erase(key);
        }
    }

    /** @return the number of key/value pairs in the map */
    size_t size() const { return m_map.size(); }

    /** Get all of the keys. Note that the map is unordered.
      * @param keys reference to a vector that will be populated with the the keys in the map
      */
    void getKeys(std::vector<K>& keys) const
    {
        keys.clear();
        auto it = m_map.begin();
        while (it != m_map.end()) {
            keys.push_back(it->first);
            ++it;
        }
    }

    /** Get all of the values. Note that the map is unordered.
      * @param values reference to a vector that will be populated with the values in the map
      */
    void getValues(std::vector<V *>& values) const
    {
        values.clear();
        auto it = m_map.begin();
        while (it != m_map.end()) {
            values.push_back(it->second);
            ++it;
        }
    }

    /** Simple iterator object for iterating over the map. Can be used to iterate keys/values without copying them */
    class Iterator
    {
        typename maptype::const_iterator iter_;
        const maptype *mapptr_;

        friend class APUObjectMap;

    public:
        /** Get whether or not the iterator is at a valid position
          * @return true if the iterator is valid
          */
        bool valid()
        {
            return mapptr_ != NULL && iter_ != mapptr_->end();
        }
        /** Get the key for the current key/value pair */
        K first() { return iter_->first; }
        /** Get a weak pointer to value for the current key/value pair */
        V * second() { return iter_->second; }

        /** Move to the next key/value pair. Use ++iter and not iter++. */
        Iterator& operator++()
        {
            ++iter_;
            return (*this);
        }
    };

    /** Get an iterator pointing to the first key/value pair in the map */
    Iterator begin() const
    {
        Iterator it;
        it.mapptr_ = &m_map;
        it.iter_ = m_map.begin();

        return it;
    }
};

#endif /* autil_objmap_h */
