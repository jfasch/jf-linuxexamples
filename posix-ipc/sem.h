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

#ifndef HAVE_JF_LINUXTOOLS_POSIX_IPC_SEM_H
#define HAVE_JF_LINUXTOOLS_POSIX_IPC_SEM_H

#include <boost/shared_ptr.hpp>

#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

namespace jf {
namespace linuxtools {

/** \brief POSIX named semaphore.
 */
class SEM
{
public:
    static SEM create(const char* path, int oflag, mode_t, unsigned int value);
    static SEM open(const char* path, int oflag);
    static void unlink(const char* path);

    void post();
    void wait();

    /** Returns true if the object is valid. */
    bool good() const { return shared_impl_.get()!=NULL; }

    /** Returns the underlying object.

        If none has been allocated, SEM_FAILED is returned.
    */
    sem_t* sem() const { return good()? shared_impl_->sem(): SEM_FAILED; }
    
private:
    class Impl
    {
    public:
        Impl(sem_t* sem) : sem_(sem) {}
        ~Impl();
        sem_t* sem() const { return sem_; }
    private:
        sem_t* sem_;
    };

    SEM(sem_t*);

private:
    boost::shared_ptr<Impl> shared_impl_;
};

}
}

#endif
