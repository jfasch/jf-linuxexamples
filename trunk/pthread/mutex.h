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

#ifndef HAVE_JF_LINUX_MUTEX_H
#define HAVE_JF_LINUX_MUTEX_H

#include <pthread.h>

namespace jflinux {

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

#endif
