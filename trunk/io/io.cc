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
#include "io.h"

namespace jflinux {

IO::IO(int fd)
: shared_fd_(new FileDescriptor(fd)) {}

IO::IO(const IO& io)
: shared_fd_(io.shared_fd_) {}

IO& IO::operator=(const IO& io)
{
    shared_fd_ = io.shared_fd_;
    return *this;
}

ssize_t IO::read(void* buf, size_t len)
{
    throw_if_null();
    return throw_if_error(::read(shared_fd_->fd(), buf, len));
}

ssize_t IO::pread(void* buf, size_t count, off_t offset)
{
    throw_if_null();
    return throw_if_error(::pread(shared_fd_->fd(), buf, count, offset));
}

ssize_t IO::write(const void* buf, size_t len)
{
    throw_if_null();
    return throw_if_error(::write(shared_fd_->fd(), buf, len));
}

ssize_t IO::pwrite(const void *buf, size_t count, off_t offset)
{
    throw_if_null();
    return throw_if_error(::pwrite(shared_fd_->fd(), buf, count, offset));
}

ssize_t IO::writeall(const void* buf, size_t len)
{
    throw_if_null();
    const ssize_t retval = len;
    const char* the_data = (const char*)buf;
    while (len) {
        int nwritten = throw_if_error(::write(shared_fd_->fd(), the_data, len));

        // man 2 write says this cannot happen as long as we don't
        // write 0 bytes.
        assert(nwritten>0);

        len -= nwritten;
        the_data += nwritten; 
    }
    return retval;
}

int IO::fd() const
{
    throw_if_null();
    return shared_fd_->fd();
}

void IO::set_fd(int fd)
{
    if (fd >=0)
        shared_fd_ = boost::shared_ptr<FileDescriptor>(new FileDescriptor(fd));
    else
        shared_fd_.reset();
}

bool IO::good() const
{
    if (shared_fd_)
        // can this be < 0 at all?
        return shared_fd_->fd() >= 0;
    return false;
}
    
// -------------------------------------------
IO::FileDescriptor::~FileDescriptor()
{
    if (fd_ >= 0)
        ::close(fd_);
}

void IO::FileDescriptor::close()
{
    ::close(fd_);
    fd_ = -1;
}   

}
