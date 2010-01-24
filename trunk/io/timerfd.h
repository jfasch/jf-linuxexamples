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

#ifndef HAVE_JFLINUX_TIMERFD_H
#define HAVE_JFLINUX_TIMERFD_H

#include "io.h"

#include <jflinux/timespec.h>

#include <sys/timerfd.h>

namespace jflinux {

class TimerFD : public IO
{
public:
    TimerFD(int clockid);
    void arm_oneshot(const TimeSpec& initial_expiration);
    void arm_periodic(const TimeSpec& initial_expiration, const TimeSpec& interval);
    void disarm();
    bool is_armed() const;
    uint64_t wait();
};
    
}

#endif
