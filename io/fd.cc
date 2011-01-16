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
#include "fd.h"

namespace jf {
namespace linuxtools {

FD::FD(int fd)
: shared_fd_(new FileDescriptor(fd)) {}

FD::FD(const FD& fd)
: shared_fd_(fd.shared_fd_) {}

FD& FD::operator=(const FD& fd)
{
    shared_fd_ = fd.shared_fd_;
    return *this;
}

ssize_t FD::read(void* buf, size_t len)
{
    assert(shared_fd_);
    ssize_t nread = ::read(shared_fd_->fd(), buf, len);
    if (nread < 0)
        throw ErrnoException(errno, "read()");
    return nread;
}

ssize_t FD::pread(void* buf, size_t count, off_t offset)
{
    assert(shared_fd_);
    ssize_t nread = ::pread(shared_fd_->fd(), buf, count, offset);
    if (nread < 0)
        throw ErrnoException(errno, "pread()");
    return nread;
}

ssize_t FD::write(const void* buf, size_t len)
{
    assert(shared_fd_);
    ssize_t nread = ::write(shared_fd_->fd(), buf, len);
    if (nread < 0)
        throw ErrnoException(errno, "write()");
    return nread;
}

ssize_t FD::pwrite(const void *buf, size_t count, off_t offset)
{
    assert(shared_fd_);
    ssize_t nread = ::pwrite(shared_fd_->fd(), buf, count, offset);
    if (nread < 0)
        throw ErrnoException(errno, "pwrite()");
    return nread;
}

ssize_t FD::writeall(const void* buf, size_t len)
{
    assert(shared_fd_);
    const ssize_t retval = len;
    const char* the_data = (const char*)buf;
    while (len) {
        int nwritten = this->write(the_data, len);

        // man 2 write says this cannot happen as long as we don't
        // write 0 bytes.
        assert(nwritten>0);

        len -= nwritten;
        the_data += nwritten; 
    }
    return retval;
}

int FD::fd() const
{
    assert(shared_fd_);
    return shared_fd_->fd();
}

void FD::set_fd(int fd)
{
    if (fd >=0)
        shared_fd_ = boost::shared_ptr<FileDescriptor>(new FileDescriptor(fd));
    else
        shared_fd_.reset();
}

bool FD::good() const
{
    if (shared_fd_)
        // can this be < 0 at all?
        return shared_fd_->fd() >= 0;
    return false;
}
    
// -------------------------------------------
FD::FileDescriptor::~FileDescriptor()
{
    if (fd_ >= 0)
        ::close(fd_);
}

void FD::FileDescriptor::close()
{
    ::close(fd_);
    fd_ = -1;
}   

}
}
