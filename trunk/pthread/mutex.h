// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef jf_unix_tools_mutex_h
#define jf_unix_tools_mutex_h

#include <pthread.h>

namespace jf {
namespace unix_tools {

class Mutex {
public:
    class Guard {
    public:
        Guard(Mutex& m) : mutex_(m) { mutex_.lock(); }
        ~Guard() { mutex_.unlock(); }
    private:
        Mutex& mutex_;
    };
    
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
    bool try_lock();

private:
    pthread_mutex_t mutex_;
    friend class Condition;

private:
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);
};

}
}

#endif
