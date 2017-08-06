//
//  autil_thread_manager.cpp
//  libblockdsp
//
//  Created by Luke on 7/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#include "autil_thread_manager.hpp"

void APUThreadManager::workerFunction(void *ctx)
{
    APUThreadManager *threadManager = (APUThreadManager *)ctx;
    do {
        threadManager->m_mtx.lock();
        if (!threadManager->m_threads.empty()) {
            std::thread *thread = threadManager->m_threads.front();
            threadManager->m_threads.pop_front();
            threadManager->m_mtx.unlock();
            
            if (thread->joinable())
                thread->join();
            
            delete thread;
        }
        threadManager->m_mtx.lock();
        if (threadManager->m_threads.empty()) {
            threadManager->m_isRunning = false;
        } else {
            threadManager->m_mtx.unlock();
        }
    } while (threadManager->m_isRunning);
    
    threadManager->m_mtx.unlock();
}

APUThreadManager::APUThreadManager()
: m_worker(NULL)
, m_isRunning(false)
{
}

APUThreadManager::~APUThreadManager()
{
    if (m_worker && m_isRunning && m_worker->joinable()) {
        m_worker->join();
        delete m_worker;
    }
}

void APUThreadManager::dispatch(dispatchable_fn func, void *ctx)
{
    m_mtx.lock();
    m_threads.push_back(new std::thread(func, ctx));
    if (!m_isRunning) {
        if (m_worker) {
            if (m_worker->joinable()) {
                m_worker->join();
            }
            delete m_worker;
        }
        m_isRunning = true;
        m_worker = new std::thread(workerFunction, this);
    }
    m_mtx.unlock();
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
