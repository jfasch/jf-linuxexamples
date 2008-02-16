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

#ifndef HAVE_JFLINUX_PTHREAD_THREAD_SPECIFIC_H
#define HAVE_JFLINUX_PTHREAD_THREAD_SPECIFIC_H

#include <pthread.h>
#include <cassert>

namespace jflinux {
namespace pthread {

/** \brief Typesafe encapsulation of thread specific data. */
template<typename T> class ThreadSpecific {
public:
    typedef void (*DTOR)(void*);

public:
    ThreadSpecific(DTOR = 0);
    ~ThreadSpecific();
    void set(T*);
    T* get();
private:
    pthread_key_t key_;
};

template<typename T> inline ThreadSpecific<T>::ThreadSpecific(DTOR dtor) {
    int err = ::pthread_key_create(&key_, dtor);
    assert(!err);
}

template<typename T> ThreadSpecific<T>::~ThreadSpecific() {
    int err = ::pthread_key_delete(key_);
    assert(!err);
}

template<typename T> void ThreadSpecific<T>::set(T* t) {
    int err = ::pthread_setspecific(key_, (void*)t);
    assert(!err);
}

template<typename T> T* ThreadSpecific<T>::get() {
    return (T*)::pthread_getspecific(key_);
}

}
}

#endif
