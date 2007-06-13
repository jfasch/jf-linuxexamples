// -*- mode: C++; c-basic-offset: 4 -*-

#include "mutex.h"

#include <cassert>

namespace jf {
namespace unix_tools {

Mutex::Mutex() {
    int err = ::pthread_mutex_init(&mutex_, NULL);
    assert(!err);
}

Mutex::~Mutex() {
    int err = ::pthread_mutex_destroy(&mutex_);
    assert(!err);
}

void Mutex::lock() {
    int err = ::pthread_mutex_lock(&mutex_);
    assert(!err);
}

void Mutex::unlock() {
    int err = ::pthread_mutex_unlock(&mutex_);
    assert(!err);
}

}
}

