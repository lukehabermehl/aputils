//
//  autil_defaultlogger.cpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include <unistd.h>
#include "autil_logger_private.hpp"

typedef APUDefaultLogger::Pimpl LoggerImpl;
typedef LoggerImpl::Queue LoggerQueue;

static const uint32_t kSleepTime_nsec = 1000;

static APUPtr<APULoggerInterface> current_logger;

void APUSetLogger(APULoggerInterface *logger)
{
    current_logger = logger;
}

APULoggerInterface * APUGetLogger()
{
    if (!current_logger) {
        current_logger = new APUDefaultLogger();
    }
    return current_logger.ptr();
}

void logger_append(LoggerImpl *loggerImpl, LoggerQueue::Item *item)
{
    if (loggerImpl != NULL && item != NULL) {
        loggerImpl->queueLock.lock();
        loggerImpl->queue->append(item);
        loggerImpl->queueLock.unlock();
    }
}

void logger_worker(LoggerImpl *loggerImpl)
{
    while (!loggerImpl->shutdown) {
        if (loggerImpl->queue == NULL) {
            struct timespec tm1, tm2;
            tm1.tv_nsec = kSleepTime_nsec;
            nanosleep(&tm1, &tm2);
            continue;
        }

        loggerImpl->queueLock.lock();
        LoggerQueue *queue = loggerImpl->queue;
        loggerImpl->queue = new LoggerQueue();
        loggerImpl->queueLock.unlock();

        LoggerQueue::Item *item = queue->first;
        while (item != NULL) {
            fprintf(item->file, "%s\n", item->str.c_str());
            if (loggerImpl->outputFile) {
                fprintf(loggerImpl->outputFile, "%s\n", item->str.c_str());
            }
            queue->popFront();
            item = queue->first;
        }

        delete queue;
    }
}

LoggerImpl::Pimpl()
{
    workerThread = std::thread(logger_worker, this);
    queue = new Queue();
}

LoggerImpl::~Pimpl()
{
    shutdown = true;
    workerThread.join();
    if (outputFile) {
        fclose(outputFile);
    }
    if (queue) {
        delete queue;
    }
}

void LoggerImpl::Queue::append(APUDefaultLogger::Pimpl::Queue::Item *item)
{
    if (first == NULL)
    {
        first = item;
        last = item;
    }
    else
    {
        last->next = item;
        last = last->next;
    }
}

void LoggerImpl::Queue::popFront()
{
    if (last == first)
    {
        delete first;
        first = NULL;
        last = NULL;
    }
    else
    {
        Item *temp = first;
        first = first->next;
        delete temp;
    }
}

APUDefaultLogger::APUDefaultLogger()
: BASE_CLASS()
{
    pimpl_ = new Pimpl();
}

APUDefaultLogger::~APUDefaultLogger()
{
    delete pimpl_;
}

void APUDefaultLogger::log(int logLevel, const char *fmt, ...)
{
    char logstr[4096];
    va_list args;
    va_start(args, fmt);
    vsprintf(logstr, fmt, args);

    logger_append(pimpl_, new LoggerQueue::Item(logstr, stdout));
}

void APUDefaultLogger::setOutputFilepath(const char *filepath)
{
    pimpl_->outputFile = fopen(filepath, "a");
}
