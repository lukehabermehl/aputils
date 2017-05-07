//
//  autil_logger_private.hpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_logger_private_h
#define autil_logger_private_h

#include <mutex>
#include <string>
#include <thread>
#include "autil_logger.hpp"

class APUDefaultLogger::Pimpl
{
public:
    Pimpl();
    virtual ~Pimpl();

    class Queue;

    Queue *queue;
    std::mutex queueLock;
    std::thread workerThread;
    FILE *outputFile;
    bool shutdown;
};

class APUDefaultLogger::Pimpl::Queue
{
public:
    struct Item {
        Item(std::string s, FILE *f)
        : str(s)
        , file(f)
        {}

        std::string str;
        FILE *file;
        Item *next;
    };

    Queue()
    : first(NULL)
    , last(NULL)
    {}

    virtual ~Queue();

    void append(Item *item);
    void popFront();

    Item *first;
    Item *last;
};

#endif /* autil_logger_private_h */
