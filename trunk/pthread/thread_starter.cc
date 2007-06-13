// -*- mode: C++; c-basic-offset: 4 -*-

#include "thread_starter.h"

#include <cstring>
#include <cassert>
#include <iostream>

using namespace std;

namespace jf {
namespace unix_tools {

ThreadStarter::ThreadStarter(Worker* w)
: joined_(false) {
    args_.SetWorker(w);
    ::memset(&thread_, 0, sizeof(thread_));
}

ThreadStarter::ThreadStarter(const Args& a)
: args_(a) {}

ThreadStarter::~ThreadStarter() {
    if (!joined_)
        join();
    delete args_.GetWorker();
}

bool ThreadStarter::start() {
    pthread_attr_t attr;
    pthread_attr_t* pattr;
    int err;

    if (args_.GetPriority() == PRIO_DEFAULT && args_.GetPolicy() == POLICY_DEFAULT)
        pattr = NULL;
    else {
        pattr = &attr;
        err = ::pthread_attr_init(&attr);
        assert(!err);
        
        switch (args_.GetPolicy()) {
            case POLICY_DEFAULT: {
                // inherit scheduling policy of the creating thread
                err = ::pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
                assert(!err);
                break;
            }
            case POLICY_FIFO: {
                err = ::pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
                assert(!err);
                err = ::pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
                assert(!err);
                break;
            }
            case POLICY_RR: {
                assert(0); // to be implemented
                break;
            };
        }

        switch (args_.GetPriority()) {
            case PRIO_LOWEST:
            case PRIO_LOWER:
            case PRIO_DEFAULT:
            case PRIO_HIGHER: {
                assert(0); // to be implemented: determine min/max
                break;
            }
            case PRIO_HIGHEST: {
                sched_param param;
                param.sched_priority = ::sched_get_priority_max(SCHED_FIFO);
                err = ::pthread_attr_setschedparam(&attr, &param);
                assert(!err);
                break;
            }
        };
    }
                
    err = ::pthread_create(&thread_, pattr, start_, this);
    if (err) {
        // hmm. need error handling badly. confix-style exceptions
        // probably?
        cerr << strerror(err) << endl;
        return false;
    }

    return true;
}

void ThreadStarter::join() {
    joined_ = true;
    int err = ::pthread_join(thread_, NULL);
    assert(!err);
}

void* ThreadStarter::start_(void* obj) {
    ThreadStarter* t = static_cast<ThreadStarter*>(obj);

    (*t->args_.GetWorker())();
    return NULL;
}

}
}
