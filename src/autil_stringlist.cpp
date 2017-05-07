//
//  autil_stringlist.cpp
//  libblockdsp
//
//  Created by Luke on 9/8/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_stringlist.hpp"
#include "autil_stringlist_private.hpp"

APUStringList::APUStringList()
{
    pimpl_ = new Pimpl();
}

APUPtr<APUStringList> APUStringList::copy()
{
    APUPtr<APUStringList> cp(new APUStringList());
    for (auto it = pimpl_->vStrings.begin(); it != pimpl_->vStrings.end(); it++)
    {
        cp->pimpl_->vStrings.push_back(*it);
    }

    return cp;
}

size_t APUStringList::size()
{
    return pimpl_->vStrings.size();
}

const char * APUStringList::getString(size_t index)
{
    if (index < size())
    {
        return pimpl_->vStrings[index].c_str();
    }

    return NULL;
}

void APUStringList::setString(size_t index, const char *str)
{
    if (index < size())
    {
        pimpl_->vStrings[index] = std::string(str);
    }
}

void APUStringList::append(const char *str)
{
    pimpl_->vStrings.push_back(std::string(str));
}
