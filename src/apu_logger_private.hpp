//
//  apu_logger_private.hpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_logger_private_h
#define apu_logger_private_h

#include "apu_logger.hpp"

#include <mutex>
#include <string>
#include <sstream>
#include <thread>

class APUDefaultLogger::Pimpl
{
public:
    Pimpl();
    virtual ~Pimpl();

    void shutdown();

    class Queue;

    Queue *queue;
    std::mutex queueLock;
    std::thread workerThread;
    FILE *outputFile;
    bool bShutdown;
    int logLevel;

};

class APUDefaultLogger::Pimpl::Queue
{
public:
    struct Item {
        Item(std::string tag, std::string s, FILE *f)
        : tag(tag)
        , str(s)
        , file(f)
        {}

        std::string tag;
        std::string str;
        FILE *file;
        Item *next;
    };

    Queue()
    : first(NULL)
    , last(NULL)
    {}

    virtual ~Queue() {
        while (first) {
            popFront();
        }
    }

    void append(Item *item);
    void popFront();

    Item *first;
    Item *last;
};

#endif /* apu_logger_private_h */
