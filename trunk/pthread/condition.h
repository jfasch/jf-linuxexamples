// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef jf_unix_tools_condition_h
#define jf_unix_tools_condition_h

#include "mutex.h"

#include <jf/unix_tools/timespec.h>

#include <pthread.h>

namespace jf {
namespace unix_tools {

class Condition {
public:
    Condition(Mutex& m);
    ~Condition();

    void wait();
    bool timed_wait(const TimeSpec& abstime);
    void signal();

private:
    pthread_cond_t cond_;
    Mutex& mutex_;

private:
    Condition(const Condition&);
    Condition& operator=(const Condition&);
};

}
}

#endif
