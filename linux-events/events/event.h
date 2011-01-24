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

#ifndef HAVE_JF_LINUXTOOLS_EVENT_H
#define HAVE_JF_LINUXTOOLS_EVENT_H

#include <jf/linuxtools/dispatchee.h>
#include <jf/linuxtools/dispatcher.h>
#include <jf/linuxtools/eventfd.h>

namespace jf {
namespace linuxtools {

/** Gives callbacks with eventfd(2) semantics.

    Add an integer, and a callback will be fired, carrying the number
    of increments.
 */
class Event : public Dispatchee,
              private Dispatcher::Handler
{
public:
    class Handler
    {
    public:
        virtual ~Handler() {}
        virtual void new_events(uint64_t) = 0;
    };
    
public:
    Event(Handler* handler): handler_(handler), dispatcher_(NULL) {}

    void add(uint64_t n) { eventfd_.add(n); }

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
    EventFD eventfd_;
    Handler* handler_;
    Dispatcher* dispatcher_;
};
    
}
}

#endif
