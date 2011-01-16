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
#include "event_suite.h"

#include <jf/linuxtools/event.h>

#include <jf/unittest/test_suite.h>
#include <jf/unittest/test_case.h>

namespace {

using namespace jf::linuxtools;

class Basic : public jf::unittest::TestCase
{
public:
    Basic() : jf::unittest::TestCase("Basic") {}
    virtual void run()
    {
        class MyHandler : public Event::Handler
        {
        public:
            MyHandler() : n_events_(0) {}
            bool n_events() const { return n_events_; }
        private:
            virtual void new_events(uint64_t n) { n_events_ += n; }
            uint64_t n_events_;
        };
        
        Dispatcher dispatcher;
        MyHandler handler;
        Event event(&handler);
        event.add(5);
        event.activate_object(&dispatcher);
        while (handler.n_events() == 0)
            dispatcher.dispatch();
        event.deactivate_object(&dispatcher);
    }
};

}

namespace jf {
namespace linuxtools {

EventSuite::EventSuite()
: jf::unittest::TestSuite("Event")
{
    add_test(new Basic);
}

}
}
