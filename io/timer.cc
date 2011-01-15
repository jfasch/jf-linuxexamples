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

#include "timer.h"

namespace jf {
namespace linuxtools {

Timer::~Timer()
{
    timerfd_.disarm();
}

void Timer::activate(Dispatcher* d)
{
    assert(dispatcher_==NULL);
    assert(d!=NULL);
    dispatcher_ = d;
    dispatcher_->watch_in(timerfd_.fd(), this);
}

void Timer::deactivate(const Dispatcher* d)
{
    assert(dispatcher_!=NULL);
    (void)d;
    assert(d==dispatcher_);

    dispatcher_->unwatch_in(timerfd_.fd(), this);
    dispatcher_ = NULL;
}

void Timer::in_ready(int fd)
{
    assert(timerfd_.fd()==fd);
    handler_->expired();
}

void Timer::out_ready(int)
{
    assert(false);
}

}
}
