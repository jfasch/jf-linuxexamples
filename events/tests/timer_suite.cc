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
#include "timer_suite.h"

#include <jf/linuxtools/timer.h>

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
        class MyHandler : public Timer::Handler
        {
        public:
            MyHandler() : called_(false) {}
            bool called() const { return called_; }
        private:
            virtual void expired() { called_ = true; }
            bool called_;
        };
        
        Dispatcher dispatcher;
        MyHandler handler;
        Timer timer(&handler);
        timer.arm_oneshot(TimeSpec(0, TimeSpec::one_second/1000));
        timer.activate_object(&dispatcher);
        while (!handler.called())
            dispatcher.dispatch();
        timer.deactivate_object(&dispatcher);
    }
};

}

namespace jf {
namespace linuxtools {

TimerSuite::TimerSuite()
: jf::unittest::TestSuite("Timer")
{
    add_test(new Basic);
}

}
}
