// -*- mode: C++; c-basic-offset: 4 -*-

#include "joinable_thread.h"

#include <cstring>
#include <cassert>
#include <iostream>

namespace jf {
namespace unix_tools {

JoinableThreadStarter::JoinableThreadStarter(Worker* w)
: joined_(false)
{
    args_.worker(w);
    ::memset(&thread_, 0, sizeof(thread_));
}

JoinableThreadStarter::JoinableThreadStarter(const Args& a)
: args_(a) {}

JoinableThreadStarter::~JoinableThreadStarter() {
    if (!joined_)
        join();
    delete args_.worker();
}

bool JoinableThreadStarter::start() {
    pthread_attr_t attr;
    pthread_attr_t* pattr;
    int err;

    if (args_.priority() == PRIO_DEFAULT && args_.policy() == POLICY_DEFAULT)
        pattr = NULL;
    else {
        pattr = &attr;
        err = ::pthread_attr_init(&attr);
        assert(!err);
        
        switch (args_.policy()) {
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
                assert(!"SCHED_RR to be implemented");
                break;
            };
        }

        switch (args_.priority()) {
            case PRIO_LOWEST:
            case PRIO_LOWER:
            case PRIO_DEFAULT:
            case PRIO_HIGHER: {
                assert(!"To be implemented: determine min/max, using sched_get_priority_{min,max}");
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
        std::cerr << ::strerror(err) << std::endl;
        return false;
    }

    return true;
}

void JoinableThreadStarter::join() {
    joined_ = true;
    int err = ::pthread_join(thread_, NULL);
    assert(!err);
}

void* JoinableThreadStarter::start_(void* obj) {
    JoinableThreadStarter* t = static_cast<JoinableThreadStarter*>(obj);

    (*t->args_.worker())();
    return NULL;
}

}
}
