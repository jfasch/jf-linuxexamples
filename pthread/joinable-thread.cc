// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2.1 of
// the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

#include "joinable-thread.h"

#include <cstring>
#include <cassert>
#include <iostream>

namespace jf {
namespace linuxtools {

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

    t->args_.worker()->run();
    return NULL;
}

}
}
