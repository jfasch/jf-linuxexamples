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
#ifndef HAVE_JFLINUX_QUEUE_H
#define HAVE_JFLINUX_QUEUE_H

#include "mutex.h"
#include "condition.h"

#include <deque>

namespace jflinux {

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

#endif
