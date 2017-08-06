//
//  autil_thread_manager.hpp
//  libblockdsp
//
//  Created by Luke on 7/30/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

///@file autil_thread_manager.hpp

#ifndef autil_thread_manager_h
#define autil_thread_manager_h

#include <thread>
#include <deque>
#include <mutex>

/** typedef for void function pointer with a single void * argument */
typedef void (*dispatchable_fn)(void *);

/** Simplify memory managment for threads */
class APUThreadManager
{
public:
    APUThreadManager();
    ~APUThreadManager();
    
    /** Get the singleton instance */
    static APUThreadManager * sharedThreadManager();
    
    /** Create a new managed thread. The memory will be automatically released when the thread exits
      * @param func function to call in the thread
      * @param ctx context pointer to pass to the function
      */
    void dispatch(dispatchable_fn func, void *ctx);
    
private:
    std::thread *m_worker;
    std::deque<std::thread *> m_threads;
    std::mutex m_mtx;
    bool m_isRunning;
    
    static void workerFunction(void *ctx);
};

/** Add a new thread to the singleton thread manager
  * @param func function to call
  * @param ctx context pointer to pass
  */
void APUDispatchThread(dispatchable_fn func, void *ctx);

#endif /* autil_thread_manager_h */
