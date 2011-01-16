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
#ifndef HAVE_JF_LINUXTOOLS_MUTEX_H
#define HAVE_JF_LINUXTOOLS_MUTEX_H

#include <boost/utility.hpp>
#include <pthread.h>

namespace jf {
namespace linuxtools {

class Mutex : public boost::noncopyable
{
public:
    class Guard
    {
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

private:
    pthread_mutex_t mutex_;
    friend class Condition;
};

}
}

#endif
