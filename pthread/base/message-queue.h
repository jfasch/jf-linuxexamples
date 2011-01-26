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

#ifndef HAVE_JF_LINUXTOOLS_QUEUE_H
#define HAVE_JF_LINUXTOOLS_QUEUE_H

#include "mutex.h"
#include "condition.h"

#include <deque>

namespace jf {
namespace linuxtools {

template<typename T> class MessageQueue {
public:
    MessageQueue(size_t maxelem);

    void push(const T&);
    void pop(T&);

private:
    size_t maxelem_;
    std::deque<T> queue_;
    Mutex mutex_;
    Condition notfull_;
    Condition notempty_;

private:
    MessageQueue(const MessageQueue&);
    MessageQueue& operator=(const MessageQueue&);
};

template<typename T> MessageQueue<T>::MessageQueue(size_t maxelem)
: maxelem_(maxelem),
  mutex_(),
  notfull_(mutex_),
  notempty_(mutex_) {}

template<typename T> void MessageQueue<T>::push(const T& elem) {
    {
        Mutex::Guard g(mutex_);
        while (queue_.size() == maxelem_)
            notfull_.wait();
        queue_.push_back(elem);
    }
    notempty_.signal();
}

template<typename T> void MessageQueue<T>::pop(T& elem) {
    {
        Mutex::Guard g(mutex_);
        while (queue_.size() == 0)
            notempty_.wait();
        elem = queue_.front();
        queue_.pop_front();
    }
    notfull_.signal();
}

}
}

#endif
