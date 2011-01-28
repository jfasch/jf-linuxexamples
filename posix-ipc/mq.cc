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

#include "mq.h"

#include <string.h>

namespace jf {
namespace linuxtools {

MQ::Attr::Attr()
{
    memset(this, 0, sizeof(*this));
}

MQ MQ::create(const char* path, int oflag, mode_t mode, const Attr& attr)
{
    oflag |= O_CREAT;

    mqd_t mq = mq_open(path, oflag, mode, &attr);
    if (mq < 0)
        throw ErrnoException(errno, "mq_open(O_CREAT)");

    MQ ret;
    ret.set_fd(mq);
    return ret;
}

MQ MQ::open(const char* path, int oflag)
{
    assert(!(oflag&O_CREAT));
    
    mqd_t mq = mq_open(path, oflag);
    if (mq < 0)
        throw ErrnoException(errno, "mq_open()");
    MQ ret;
    ret.set_fd(mq);
    return ret;
}

void MQ::send(const char* msg, size_t msg_len, unsigned priority)
{
    if (mq_send(this->fd(), msg, msg_len, priority) < 0)
        throw ErrnoException(errno, "mq_send()");
}

size_t MQ::receive(char* msg, size_t msg_len)
{
    unsigned prio;
    ssize_t nread = mq_receive(this->fd(), msg, msg_len, &prio);
    if (nread < 0)
        throw ErrnoException(errno, "mq_receive()");
    return nread;
}

}
}
