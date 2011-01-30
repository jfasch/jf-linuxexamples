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

#ifndef HAVE_JF_LINUXTOOLS_POSIX_IPC_SHM_H
#define HAVE_JF_LINUXTOOLS_POSIX_IPC_SHM_H

#include <jf/linuxtools/fd.h>

#include <string>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

namespace jf {
namespace linuxtools {

/** \brief POSIX shared memory.
 */
class SHM : public FD
{
public:
    static SHM create(const char* path, int oflag, mode_t, off_t size);
    static SHM create(const std::string& path, int oflag, mode_t, off_t size);

    static SHM open(const char* path, int oflag);
    static SHM open(const std::string& path, int oflag);

    static void unlink(const char* path);
    static void unlink(const std::string& path);

    /** Direct mmap() on the underlying file descriptor.

        The first parameter to mmap(), the address is omitted because
        nobody uses it anyway.
     */
    void* map(size_t length, int prot, int flags, off_t offset);
};

}
}

#endif
