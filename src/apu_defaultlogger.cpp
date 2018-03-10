//
//  apu_defaultlogger.cpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#include "apu_logger_private.hpp"
#include <unistd.h>

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
    static struct timespec sleepTime;
    sleepTime.tv_nsec = kSleepTime_nsec;

    while (true) {
        bool bShutdown = false;
        loggerImpl->queueLock.lock();
        bShutdown = loggerImpl->bShutdown;
        if (bShutdown) {
            loggerImpl->queueLock.unlock();
            break;
        }
        if (loggerImpl->queue == NULL) {
            loggerImpl->queueLock.unlock();
            nanosleep(&sleepTime, NULL);
            continue;
        }

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
    bool isShutdown = false;
    queueLock.lock();
    isShutdown = bShutdown;
    queueLock.unlock();
    if (!isShutdown) {
        shutdown();
    }
    if (outputFile) {
        fclose(outputFile);
    }
    if (queue) {
        delete queue;
    }
}

void
LoggerImpl::shutdown()
{
    queueLock.lock();
    bShutdown = true;
    queueLock.unlock();

    if (workerThread.joinable()) {
        workerThread.join();
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

void APUDefaultLogger::shutdown()
{
    m_pimpl->shutdown();
}
