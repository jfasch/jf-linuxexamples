// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2010-2011 Joerg Faschingbauer

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

#include "timerfd.h"

#include <jf/linuxtools/error.h>

#include <cstring>
#include <unistd.h>

namespace jf {
namespace linuxtools {

TimerFD::TimerFD(clockid_t clockid)
{
    int fd = ::timerfd_create(clockid, 0);
    if (fd < 0)
        throw ErrnoException(errno);
    set_fd(fd);
}

void TimerFD::arm_oneshot(const TimeSpec& initial_expiration)
{
    assert(this->fd()>=0);

    itimerspec new_value, old_value;
    std::memset(&new_value, 0, sizeof(itimerspec));
    std::memset(&old_value, 0, sizeof(itimerspec));
    new_value.it_value = initial_expiration;
    int retval = ::timerfd_settime(this->fd(), 0, &new_value, &old_value);
    if (retval < 0)
        throw ErrnoException(errno);
}

void TimerFD::arm_periodic(const TimeSpec& initial_expiration, const TimeSpec& interval)
{
    assert(this->fd()>=0);

    itimerspec new_value, old_value;
    std::memset(&new_value, 0, sizeof(itimerspec));
    std::memset(&old_value, 0, sizeof(itimerspec));
    new_value.it_value = initial_expiration;
    new_value.it_interval = interval;
    int retval = ::timerfd_settime(this->fd(), 0, &new_value, &old_value);
    if (retval < 0)
        throw ErrnoException(errno);
}

void TimerFD::disarm()
{
    assert(this->fd()>=0);

    itimerspec new_value, old_value;
    std::memset(&new_value, 0, sizeof(itimerspec));
    std::memset(&old_value, 0, sizeof(itimerspec));
    int retval = ::timerfd_settime(this->fd(), 0, &new_value, &old_value);
    if (retval < 0)
        throw ErrnoException(errno);
}

bool TimerFD::is_armed() const
{
    assert(this->fd()>=0);
    
    itimerspec curr_value;
    int retval = ::timerfd_gettime(this->fd(), &curr_value);
    if (retval < 0)
        throw ErrnoException(errno);

    return !(curr_value.it_value.tv_sec == 0 && curr_value.it_value.tv_nsec == 0);
}

uint64_t TimerFD::wait()
{
    assert(this->fd()>=0);

    uint64_t num_expirations;
    ssize_t nread = ::read(this->fd(), &num_expirations, sizeof(num_expirations));
    assert(nread==sizeof(num_expirations));
    return num_expirations;
}

}
}
