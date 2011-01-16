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
#ifndef HAVE_JF_LINUXTOOLS_FD_H
#define HAVE_JF_LINUXTOOLS_FD_H

#include <jf/linuxtools/error.h>

// CONFIX:REQUIRE_H('boost/shared_ptr.hpp', REQUIRED)
#include <boost/shared_ptr.hpp>

#include <errno.h>

namespace jf {
namespace linuxtools {

/** Base class for all Unix style file descriptor objects.

Under Unix almost everything is a "file". This is what this class
encapsulates: common operations on a file - read and write -, as well
as the file descriptor itself.

Copy semantics of this class (and all inherited classes) is
"refcounting". That is, one can copy FD objects around at will, and
the last one that is deallocated will release the file descriptor -
i.e. close() it.

*/
class FD
{
public:
    /** Create an "empty" FD object that has no file descriptor
        yet. */
    FD() {}
    /** Create a new file object which is wrapped around file
        descriptor fd. */
    FD(int fd);
    /** Copy an existing FD object. Increases the refcount of the
        underlying file descriptor if any. */
    //@{
    FD(const FD&);
    FD& operator=(const FD&);
    //@}

    /** Get the file descriptor. If the FD object is invalid, -1 is
        returned. */
    int fd() const;
    /** Release the reference to an existing file descriptor if
        any. Start maintaining fd as underlying file descriptor. If fd
        is a negative value, only the reference is released. */
    void set_fd(int fd);
    /** A wrapper around the read(2) system call. */
    ssize_t read(void*, size_t);
    /** A wrapper around the pread(2) system call. */
    ssize_t pread(void* buf, size_t count, off_t offset);
    /** A wrapper around the write(2) system call. */
    ssize_t write(const void*, size_t);
    /** A wrapper around the pwrite(2) system call. */
    ssize_t pwrite(const void *buf, size_t count, off_t offset);
    
    /** Call write(2) in a loop until all bytes are written or an
        error is encountered. */
    ssize_t writeall(const void*, size_t);
    /** Close the underlying file descriptor the hard way. This means
        that all copies of this will be useless from here on. */
    void close() { if (shared_fd_) shared_fd_->close(); }

    /** Returns true if we maintain a file descriptor, and if so, if
        it is valid. */
    bool good() const;

private:
    class FileDescriptor
    {
    public:
        FileDescriptor(int fd) : fd_(fd) {}
        ~FileDescriptor();
        int fd() const { return fd_; }
        void close();
    private:
        int fd_;
    };

private:
    inline void throw_if_null() const
    {
        if (!shared_fd_) throw ErrnoException(EBADF);
    }
    inline ssize_t throw_if_error(ssize_t n) const
    {
        if (n < 0) throw ErrnoException(errno);
        return n;
    }

private:    
    boost::shared_ptr<FileDescriptor> shared_fd_;
};
    
}
}

#endif
