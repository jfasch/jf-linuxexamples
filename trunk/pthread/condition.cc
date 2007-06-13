// -*- mode: C++; c-basic-offset: 4 -*-

#include "condition.h"

#include <cassert>
#include <cerrno>

namespace jf {
namespace unix_tools {

Condition::Condition(Mutex& m)
: mutex_(m) {
    int err = ::pthread_cond_init(&cond_, NULL);
    assert(!err);
}

Condition::~Condition() {
    int err = ::pthread_cond_destroy(&cond_);
    assert(!err);
}

void Condition::wait() {
    int err = ::pthread_cond_wait(&cond_, &mutex_.mutex_);
    assert(!err);
}

bool Condition::timed_wait(const TimeSpec& abstime) {
    int err = ::pthread_cond_timedwait(&cond_, &mutex_.mutex_, &abstime);
    if (err) {
        if (err == ETIMEDOUT)
            return true;
        assert(false);
    }
    return false;
}

void Condition::signal() {
    int err = ::pthread_cond_signal(&cond_);
    assert(!err);
}

}
}
