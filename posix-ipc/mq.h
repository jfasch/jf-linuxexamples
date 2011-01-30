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

#ifndef HAVE_JF_LINUXTOOLS_POSIX_IPC_MQ_H
#define HAVE_JF_LINUXTOOLS_POSIX_IPC_MQ_H

#include <jf/linuxtools/fd.h>

#include <string>
#include <mqueue.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

namespace jf {
namespace linuxtools {

/** \brief POSIX message queue.

    Under Linux, POSIX message queue handles are in fact file
    descriptors - and we treat them as such.
 */
class MQ : public FD
{
public:
    struct Attr : public mq_attr
    {
        Attr();
        Attr& set_maxmsg(long l) { mq_maxmsg = l; return *this; }
        Attr& set_msgsize(long l) { mq_msgsize = l; return *this; }
    };

    static MQ create(const char* path, int oflag, mode_t, const Attr&);
    static MQ create(const std::string& path, int oflag, mode_t, const Attr&);

    static MQ open(const char* path, int oflag);
    static MQ open(const std::string& path, int oflag);

    static void unlink(const char* path);
    static void unlink(const std::string& path);

public:
    MQ() : max_msgsize_(-1) {}

    long max_msgsize() const { return max_msgsize_; }
    
    void send(const char* msg, size_t msg_len, unsigned priority);
    size_t receive(char* msg, size_t msg_len);

private:
    long max_msgsize_;
};

}
}

#endif
