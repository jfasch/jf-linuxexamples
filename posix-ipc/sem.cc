// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2011 Joerg Faschingbauer

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

#include "sem.h"

#include <jf/linuxtools/error.h>

#include <errno.h>

namespace jf {
namespace linuxtools {

SEM::SEM(sem_t* sem)
: shared_impl_(new Impl(sem)) {}

SEM SEM::create(const char* path, int oflag, mode_t mode, unsigned int value)
{
    oflag |= O_CREAT;

    sem_t* sem = sem_open(path, oflag, mode, value);
    if (sem == SEM_FAILED)
        throw ErrnoException(errno, "sem_open(O_CREAT)");

    return SEM(sem);
}

SEM SEM::create(const std::string& path, int oflag, mode_t mode, unsigned int value)
{
    return create(path.c_str(), oflag, mode, value);
}

SEM SEM::open(const char* path, int oflag)
{
    assert(!(oflag&O_CREAT));

    sem_t* sem = sem_open(path, oflag);
    if (sem < 0)
        throw ErrnoException(errno, "sem_open()");

    return SEM(sem);
}

SEM SEM::open(const std::string& path, int oflag)
{
    return open(path.c_str(), oflag);
}

void SEM::unlink(const char* path)
{
    if (sem_unlink(path) < 0)
        throw ErrnoException(errno, "sem_unlink()");
}

void SEM::unlink(const std::string& path)
{
    unlink(path.c_str());
}

void SEM::post()
{
    assert(this->good());

    if (sem_post(this->sem()) < 0)
        throw ErrnoException(errno, "sem_post()");
}

void SEM::wait()
{
    assert(this->good());

    if (sem_wait(this->sem()) < 0)
        throw ErrnoException(errno, "sem_wait()");
}

SEM::Impl::~Impl()
{
    sem_close(sem_);
}

}
}
