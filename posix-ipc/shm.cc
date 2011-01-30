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

#include "shm.h"

namespace jf {
namespace linuxtools {

SHM SHM::create(const char* path, int oflag, mode_t mode, off_t size)
{
    oflag |= O_CREAT;

    int fd = shm_open(path, oflag, mode);
    if (fd < 0)
        throw ErrnoException(errno, "shm_open(O_CREAT)");

    SHM ret;
    ret.set_fd(fd);

    if (ftruncate(fd, size) < 0)
        throw ErrnoException(errno, "ftruncate()");
    
    return ret;
}

SHM SHM::create(const std::string& path, int oflag, mode_t mode, off_t size)
{
    return create(path.c_str(), oflag, mode, size);
}

SHM SHM::open(const char* path, int oflag)
{
    assert(!(oflag&O_CREAT));
    
    int fd = shm_open(path, oflag, 0);
    if (fd < 0)
        throw ErrnoException(errno, "shm_open()");
    SHM ret;
    ret.set_fd(fd);
    return ret;
}

SHM SHM::open(const std::string& path, int oflag)
{
    return open(path.c_str(), oflag);
}

void SHM::unlink(const char* path)
{
    if (shm_unlink(path) < 0)
        throw ErrnoException(errno, "shm_unlink()");
}

void SHM::unlink(const std::string& path)
{
    unlink(path.c_str());
}

void* SHM::map(size_t length, int prot, int flags, off_t offset)
{
    void* ret = mmap(NULL, length, prot, flags, this->fd(), offset);
    if (ret == MAP_FAILED)
        throw ErrnoException(errno, "mmap()");
    return ret;
}

}
}
