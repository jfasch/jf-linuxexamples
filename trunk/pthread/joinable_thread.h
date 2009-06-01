// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008 Joerg Faschingbauer

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

#ifndef HAVE_JFLINUX_PTHREAD_JOINABLE_THREAD_H
#define HAVE_JFLINUX_PTHREAD_JOINABLE_THREAD_H

#include <pthread.h>

namespace jflinux {
namespace pthread {

/**

A JoinableThreadStarter object is used to start a thread, and to
synchronize with its termination. From the user's point of view, the
JoinableThreadStarter object itself runs in only one context - namely
the user's (the user is the one who starts the thread).

A Worker object is responsible for the real hard work; this is done in
the thread's context. So, generally, the user doesn't have to worry
about what runs in which context. He can customize the thread's
behavior by simply implementing an interface, Worker, and passing it
to a thread starter object. The user's object will run in the new
thread, and nowhere else.

Note that the Worker object is not deleted; this has to be done by the
user (he needs to join() anyway).

*/
class JoinableThreadStarter {
public:
    enum Priority {
      PRIO_LOWEST,
      PRIO_LOWER,
      PRIO_DEFAULT,
      PRIO_HIGHER,
      PRIO_HIGHEST
    };

    enum Policy {
      POLICY_DEFAULT,
      POLICY_FIFO,
      POLICY_RR
    };

    class Worker {
    public:
        virtual ~Worker() {}
        /** Yeah, doit baby! */
        virtual void run() = 0;
    };

    class Args {
    public:
        Args() : worker_(NULL), priority_(PRIO_DEFAULT), policy_(POLICY_DEFAULT) {}

        Worker* worker() const { return worker_; }
        Args& worker(Worker* w) { worker_=w; return *this; }

        Priority priority() const { return priority_; }
        Args& priority(Priority p) { priority_=p; return *this; }

        Policy policy() const { return policy_; }
        Args& policy(Policy p) { policy_=p; return *this; }

    private:
        Worker* worker_;
        Priority priority_;
        Policy policy_;
    };

public:
    /** Take ownership of the worker (delete it in the dtor). Do
        nothing else but wait until you are started. */
    JoinableThreadStarter(Worker*);
    /** Enhanced version of the constructor, accepting "named"
        parameters. */
    JoinableThreadStarter(const Args&);
    /** Synchronizes with the thread's termination. I.e., blocks until
        the thread has terminated. */
    ~JoinableThreadStarter();

    /** Start the thread. */
    bool start();
    /** Wait until the thread has terminated. */
    void join();

private:
    static void* start_(void*);

private:
    pthread_t thread_;
    bool joined_;
    Args args_;
};

}
}

#endif
