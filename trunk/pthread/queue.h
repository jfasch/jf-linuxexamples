// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef jf_unix_tools_queue_h
#define jf_unix_tools_queue_h

#include "mutex.h"
#include "condition.h"

#include <deque>

namespace jf {
namespace unix_tools {

template<typename T> class Queue {
public:
    Queue(size_t maxelem);

    void push(const T&);
    void pop(T&);

    /** Wait until abstime is reached (or an element arrives, of
        course). Return true if abstime has been reached, else false
        (which means that an element was popped form the queue). */
    bool timed_pop(T&, const TimeSpec& abstime);

private:
    size_t maxelem_;
    std::deque<T> queue_;
    Mutex mutex_;
    Condition notfull_;
    Condition notempty_;

private:
    Queue(const Queue&);
    Queue& operator=(const Queue&);
};

template<typename T> Queue<T>::Queue(size_t maxelem)
: maxelem_(maxelem),
  mutex_(),
  notfull_(mutex_),
  notempty_(mutex_) {}

template<typename T> void Queue<T>::push(const T& elem) {
    {
        Mutex::Guard g(mutex_);
        while (queue_.size() == maxelem_)
            notfull_.wait();
        queue_.push_back(elem);
    }
    notempty_.signal();
}

template<typename T> void Queue<T>::Pop(T& elem) {
    {
        Mutex::Guard g(mutex_);
        while (queue_.size() == 0)
            notempty_.wait();
        elem = queue_.front();
        queue_.pop_front();
    }
    notfull_.signal();
}

template<typename T> bool Queue<T>::timed_pop(T& elem, const TimeSpec& abstime) {
    {
        Mutex::Guard g(mutex_);
        while (queue_.size() == 0) {
            bool timedout = notempty_.timed_wait(abstime);
            if (timedout)
                return true;
        }
        elem = queue_.front();
        queue_.pop_front();
    }
    notfull_.signal();
    return false;
}

}
}

#endif
