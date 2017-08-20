//
//  apu_defaultlogger.cpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include <unistd.h>
#include "apu_logger_private.hpp"

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
            fprintf(item->file, "%s %s\n", item->tag.c_str(), item->str.c_str());
            if (loggerImpl->outputFile) {
                fprintf(loggerImpl->outputFile, "%s %s\n", item->tag.c_str(), item->str.c_str());
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
    outputFile = NULL;
    queue = new Queue();
#ifdef DEBUG
    logLevel = LOG_LEVEL_DEBUG;
#else
    logLevel = LOG_LEVEL_ERROR;
#endif
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
    m_pimpl = new Pimpl();
}

APUDefaultLogger::~APUDefaultLogger()
{
    delete m_pimpl;
}

void APUDefaultLogger::log(const char *tag, int logLevel, const char *fmt, ...)
{
    if (logLevel > m_pimpl->logLevel) {
        return;
    }

    char logstr[4096];
    logstr[4095] = '\0';
    va_list args;
    va_start(args, fmt);
    vsprintf(logstr, fmt, args);

    logger_append(m_pimpl, new LoggerQueue::Item(tag, logstr, stdout));
}

void APUDefaultLogger::setOutputFilepath(const char *filepath)
{
    m_pimpl->outputFile = fopen(filepath, "a");
}

void APUDefaultLogger::setLogLevel(int level)
{
    m_pimpl->logLevel = level;
}
