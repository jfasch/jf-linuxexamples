// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2010 Joerg Faschingbauer

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

#include "eventfd.h"

#include <jf/linuxtools/error.h>

#include <unistd.h>

namespace jf {
namespace linuxtools {

EventFD::EventFD(unsigned int initval)
{
    int fd = ::eventfd(initval, 0);
    if (fd < 0)
        throw ErrnoException(errno);
    set_fd(fd);
}

void EventFD::add(uint64_t value)
{
    assert(this->fd()>=0);
    ssize_t nwritten = ::write(this->fd(), &value, sizeof(value));
    assert(nwritten==sizeof(value));
}

uint64_t EventFD::reset()
{
    assert(this->fd()>=0);
    uint64_t value;
    ssize_t nread = ::read(this->fd(), &value, sizeof(value));
    assert(nread==sizeof(value));
    return value;
}

}
}
