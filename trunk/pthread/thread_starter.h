// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef jf_unix_tools_thread_starter_h
#define jf_unix_tools_thread_starter_h

#include <pthread.h>

namespace jf {
namespace unix_tools {

/**

A ThreadStarter object is used to start a thread, and to synchronize with its
termination. From the user's point of view, the ThreadStarter object itself
runs in only one context - namely the user's (the user is the one who
starts the thread).

A Worker object is responsible for the real hard work; this is done in
the thread's context. So, generally, the user doesn't have to worry
about what runs in which context. He can customize the thread's
behavior by simply implementing an interface and passing it to a
thread manager object. The user's object will run in the new thread,
ond nowhere else.

*/
class ThreadStarter {
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
        virtual void operator()() = 0;
    };

    class Args {
    public:
        Args() : worker_(NULL), priority_(PRIO_DEFAULT), policy_(POLICY_DEFAULT) {}
        Worker* GetWorker() const { return worker_; }
        Args& SetWorker(Worker* w) { worker_=w; return *this; }
        Priority GetPriority() const { return priority_; }
        Policy GetPolicy() const { return policy_; }
        Args& SetPriority(Priority p) { priority_=p; return *this; }
        Args& SetPolicy(Policy p) { policy_=p; return *this; }
    private:
        Worker* worker_;
        Priority priority_;
        Policy policy_;        
    };

public:
    /** Take ownership of the worker (delete it in the dtor). Do
     * nothing else but wait until you are started. */
    ThreadStarter(Worker*);
    /** Enhanced version of the constructor, accepting "named"
     * parameters. */
    ThreadStarter(const Args&);
    /** Synchronizes with the thread's termination. I.e., blocks until
        the thread has terminated. */
    ~ThreadStarter();

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
