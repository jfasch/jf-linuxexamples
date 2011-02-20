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

#include "timer-suite.h"

#include <jf/linuxtools/timer.h>

#include <jf/unittest/test_suite.h>
#include <jf/unittest/test_case.h>

namespace {

using namespace jf::linuxtools;

class MyHandler : public Timer::Handler
{
public:
    MyHandler() : n_expires_(0) {}
    uint64_t n_expires() const { return n_expires_; }
private:
    virtual void expired(uint64_t n_expires)
    {
        n_expires_ += n_expires;
    }
    uint64_t n_expires_;
};

class OneShotOneTime : public jf::unittest::TestCase
{
public:
    OneShotOneTime() : jf::unittest::TestCase("OneShotOneTime") {}
    virtual void run()
    {
        Dispatcher dispatcher;
        MyHandler handler;
        Timer timer(&handler);
        timer.arm_oneshot(TimeSpec(0, TimeSpec::one_second/1000));
        timer.activate_object(&dispatcher);
        while (handler.n_expires() != 1)
            dispatcher.dispatch();
        timer.deactivate_object(&dispatcher);
    }
};

class OneShotMultipleTimes : public jf::unittest::TestCase
{
public:
    OneShotMultipleTimes() : jf::unittest::TestCase("OneShotMultipleTimes") {}
    virtual void run()
    {
        Dispatcher dispatcher;
        MyHandler handler;
        Timer timer(&handler);
        timer.activate_object(&dispatcher);

        timer.arm_oneshot(TimeSpec(0, TimeSpec::one_second/1000));
        while (handler.n_expires() != 1)
            dispatcher.dispatch();

        timer.arm_oneshot(TimeSpec(0, TimeSpec::one_second/1000));
        while (handler.n_expires() != 2)
            dispatcher.dispatch();

        timer.deactivate_object(&dispatcher);
    }
};

class PeriodicMultipleTimes : public jf::unittest::TestCase
{
public:
    PeriodicMultipleTimes() : jf::unittest::TestCase("PeriodicMultipleTimes") {}
    virtual void run()
    {
        Dispatcher dispatcher;
        MyHandler handler;
        Timer timer(&handler);
        timer.activate_object(&dispatcher);
        timer.arm_periodic(TimeSpec(0,1), TimeSpec(0, TimeSpec::one_second/1000));

        while (handler.n_expires() < 2)
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
    add_test(std::auto_ptr<Test>(new OneShotOneTime));
    add_test(std::auto_ptr<Test>(new OneShotMultipleTimes));
    add_test(std::auto_ptr<Test>(new PeriodicMultipleTimes));
}

}
}
