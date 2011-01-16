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

#ifndef HAVE_JF_LINUXTOOLS_TIMER_H
#define HAVE_JF_LINUXTOOLS_TIMER_H

#include "dispatchee.h"
#include "dispatcher.h"

#include <jf/linuxtools/timerfd.h>

namespace jf {
namespace linuxtools {

/** Adds callbacks to TimerFD. */
class Timer : public Dispatchee,
              private Dispatcher::Handler
{
public:
    class Handler
    {
    public:
        virtual ~Handler() {}
        virtual void expired() = 0;
    };
    
public:
    Timer(Handler* handler): handler_(handler), dispatcher_(NULL) {}
    virtual ~Timer();

    void arm_oneshot(const TimeSpec& initial_expiration)
    {
        timerfd_.arm_oneshot(initial_expiration);
    }
    void arm_periodic(const TimeSpec& initial_expiration, const TimeSpec& interval)
    {
        timerfd_.arm_periodic(initial_expiration, interval);
    }
    void disarm()
    { 
        timerfd_.disarm();
    }
    bool is_armed() const
    {
        return timerfd_.is_armed();
    }

public:
    /** Dispatchee implementation. */
    //@{
    virtual void activate_object(Dispatcher*);
    virtual void deactivate_object(const Dispatcher*);
    //@}

private:
    /** Dispatcher::handler implementation. */
    //@{
    virtual void in_ready(int);
    virtual void out_ready(int);
    //@}

private:
    TimerFD timerfd_;
    Handler* handler_;
    Dispatcher* dispatcher_;
};
    
}
}

#endif
