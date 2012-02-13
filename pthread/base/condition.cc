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

#include "condition.h"

#include <cassert>
#include <cerrno>

namespace jf {
namespace linuxtools {

Condition::Condition()
{
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    int err = ::pthread_cond_init(&cond_, &attr);
    assert(!err);
    err = pthread_condattr_destroy(&attr);
    assert(!err);
}

Condition::~Condition() {
    int err = ::pthread_cond_destroy(&cond_);
    assert(!err);
}

void Condition::wait(Mutex& m) {
    int err = ::pthread_cond_wait(&cond_, &m.mutex_);
    assert(!err);
}

bool Condition::timed_wait_absolute(Mutex& m, const TimeSpec& wait_time) {
    int err = ::pthread_cond_timedwait(&cond_, &m.mutex_, &wait_time);
    if (err) {
        if (err == ETIMEDOUT)
            return true;
        assert(false);
    }
    return false;
}

bool Condition::timed_wait_relative(Mutex& m, const TimeSpec& wait_time) {
    jf::linuxtools::TimeSpec then(jf::linuxtools::TimeSpec::now_monotonic() + wait_time);
    return timed_wait_absolute(m, then);
}

void Condition::signal() {
    int err = ::pthread_cond_signal(&cond_);
    assert(!err);
}

}
}
