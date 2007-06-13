// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef jf_unix_tools_thread_specific_h
#define jf_unix_tools_thread_specific_h

#include <pthread.h>
#include <cassert>

namespace jf {
namespace unix_tools {

/** \brief Typesafe encapsulation of thread specific data. */
template<typename T> class ThreadSpecific {
public:
    typedef void (*DTOR)(void*);

public:
    ThreadSpecific(DTOR = 0);
    ~ThreadSpecific();
    void Set(T*);
    T* Get();
private:
    pthread_key_t key_;
} ;


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
