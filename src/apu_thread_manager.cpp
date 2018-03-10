//
//  apu_thread_manager.cpp
//  libaputils
//
//  Created by Luke on 7/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "apu_thread_manager.hpp"

void APUThreadManager::workerFunction(void *ctx)
{
    APUThreadManager *threadManager = (APUThreadManager *)ctx;
    static struct timespec sleepTime;
    sleepTime.tv_nsec = 1000;
    sleepTime.tv_sec = 0;

    do {
        threadManager->m_workerMtx.lock();
        bool running = threadManager->m_isShutdown;
        threadManager->m_workerMtx.unlock();
        if (!running) {
            break;
        }
        threadManager->m_mtx.lock();
        if (!threadManager->m_threads.empty()) {
            std::thread *thread = threadManager->m_threads.front();
            threadManager->m_threads.pop_front();
            threadManager->m_mtx.unlock();
            
            if (thread->joinable())
                thread->join();
            
            delete thread;

            threadManager->m_mtx.lock();
        }
        if (threadManager->m_threads.empty()) {
            threadManager->m_isRunning = false;
        }
        threadManager->m_mtx.unlock();
        nanosleep(&sleepTime, NULL);
    } while (true);
}

APUThreadManager::APUThreadManager()
: m_worker(NULL)
, m_isRunning(false)
, m_isShutdown(false)
{
}

APUThreadManager::~APUThreadManager()
{
    if (m_isRunning) {
        shutdown();
    }
}

void APUThreadManager::dispatch(dispatchable_fn func, void *ctx)
{
    m_mtx.lock();
    if (m_isShutdown) {
        m_mtx.unlock();
        return;
    }
    m_workerMtx.lock();
    m_threads.push_back(new std::thread(func, ctx));
    if (!m_isRunning) {
        m_workerMtx.unlock();
        if (m_worker) {
            if (m_worker->joinable()) {
                m_worker->join();
            }
            delete m_worker;
        }
        m_workerMtx.lock();
        m_isRunning = true;
        m_worker = new std::thread(workerFunction, this);
        m_workerMtx.unlock();
    }
    else {
        m_workerMtx.unlock();
    }
    m_mtx.unlock();
}

void
APUThreadManager::shutdown()
{
    m_mtx.lock();
    if (m_isShutdown) {
        m_mtx.unlock();
        return;
    }

    m_mtx.unlock();
    m_workerMtx.lock();
    m_isRunning = false;
    m_workerMtx.unlock();
    m_mtx.lock();
    if (m_worker->joinable()) {
        m_worker->join();
    }

    while (!m_threads.empty()) {
        std::thread *thr = m_threads.front();
        m_threads.pop_front();

        if (thr->joinable()) {
            thr->join();
        }

        delete thr;
    }

    delete m_worker;
    m_worker = NULL;
    m_isShutdown = true;

    m_mtx.unlock();
}

bool
APUThreadManager::isShutdown()
{
    return m_isShutdown;
}

APUThreadManager * APUThreadManager::sharedThreadManager()
{
    static APUThreadManager singleton;
    
    return &singleton;
}

void APUDispatchThread(dispatchable_fn func, void *ctx)
{
    APUThreadManager::sharedThreadManager()->dispatch(func, ctx);
}
