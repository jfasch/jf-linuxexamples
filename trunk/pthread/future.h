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
// (C) Atronic -*- C++ -*-

#ifndef HAVE_JFLINUX_PTHREAD_FUTURE_H
#define HAVE_JFLINUX_PTHREAD_FUTURE_H

#include "mutex.h"
#include "condition.h"

#include <boost/utility.hpp>

namespace jflinux {
namespace pthread {

/** A Future is a synchronization/communication mechanism around a
    "return value". A consumer thread waits for a value that another
    (producer) thread will produce at any time in the future. */
template <typename DATA> class Future : public boost::noncopyable
{
public:
    Future() : result_valid_(false), result_ready_(lock_) {}

    /** Wait for the result to become valid. */
    const DATA& get()
    {
        Mutex::Guard g(lock_);
        while (!result_valid_)
            result_ready_.wait();
        return result_;
    }

    /** Make the result valid and wakeup the one who's waiting for it,
        if any. */
    void set(const DATA& result)
    {
        lock_.lock();
        result_valid_ = true;
        result_ = result;
        lock_.unlock();
        result_ready_.signal();
    }
    
private:
    Mutex lock_;
    DATA result_;
    bool result_valid_;
    Condition result_ready_;
};
    
}
}

#endif
