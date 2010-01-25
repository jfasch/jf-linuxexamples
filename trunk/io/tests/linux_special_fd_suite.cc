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

#include "linux_special_fd_suite.h"

#include <jflinux/eventfd.h>
#include <jflinux/timerfd.h>
#include <jflinux/signalfd.h>
#include <jflinux/timespec.h>
#include <jflinux/sigset.h>

#include <jf/unittest/test_case.h>

#include <iostream>
#include <signal.h>

namespace {

using namespace jflinux;

class EventFDTest : public jf::unittest::TestCase
{
public:
    EventFDTest() : jf::unittest::TestCase("EventFD") {}

    virtual void run()
    {
        EventFD event_fd;
        event_fd.add(100);
        event_fd.add(100);
        JFUNIT_ASSERT(event_fd.reset() == 200);
    }
};

class TimerFDTest : public jf::unittest::TestCase
{
public:
    TimerFDTest() : jf::unittest::TestCase("TimerFD") {}

    virtual void run()
    {
        // one-shot expiration.
        {
            TimeSpec wait_time(0, TimeSpec::one_second/10);
            TimerFD timer_fd(CLOCK_MONOTONIC);
            timer_fd.arm_oneshot(wait_time);
            JFUNIT_ASSERT(timer_fd.is_armed());
            TimeSpec before = TimeSpec::now_monotonic();
            uint64_t num_expirations = timer_fd.wait();
            JFUNIT_ASSERT(num_expirations == 1);
            JFUNIT_ASSERT(TimeSpec::now_monotonic() - before >= wait_time);
            JFUNIT_ASSERT(!timer_fd.is_armed());
        }

        // one-shot, disabled prematurely.
        {
            TimeSpec wait_time(100, 0);
            TimerFD timer_fd(CLOCK_MONOTONIC);
            timer_fd.arm_oneshot(wait_time);
            JFUNIT_ASSERT(timer_fd.is_armed());
            timer_fd.disarm();
            JFUNIT_ASSERT(!timer_fd.is_armed());
        }

        // periodic expiration
        {
            TimeSpec initial(0,1);
            TimeSpec interval(0,1);
            TimerFD timer_fd(CLOCK_MONOTONIC);
            timer_fd.arm_periodic(initial, interval);
            int nexpires = 5;
            while (nexpires > 0)
                nexpires -= timer_fd.wait();
        }
    }
};

class SignalFDTest : public jf::unittest::TestCase
{
public:
    SignalFDTest() : jf::unittest::TestCase("SignalFD") {}

    virtual void setup()
    {
        SigSet blocked;
        blocked.add(SIGTERM);

        SigSet old_blocked;
        int rv = ::sigprocmask(SIG_BLOCK, &blocked, &old_blocked);
        assert(rv>=0);
        unblock_sigterm_ = ! ::sigismember(&old_blocked, SIGTERM);
    }

    virtual void teardown()
    {
        SigSet cur_blocked;
        int rv = ::sigprocmask(SIG_BLOCK, NULL, &cur_blocked);
        assert(rv>=0);
        if (unblock_sigterm_) {
            cur_blocked.del(SIGTERM);
            rv = ::sigprocmask(SIG_UNBLOCK, &cur_blocked, NULL);
            assert(rv>=0);
        }
    }

    virtual void run()
    {
        SigSet signals;
        signals.add(SIGTERM);
        SignalFD signal_fd(signals);
        ::kill(getpid(), SIGTERM);
        signalfd_siginfo info;
        signal_fd.wait(info);
        JFUNIT_ASSERT(info.ssi_signo == SIGTERM);
    }

private:
    bool unblock_sigterm_;
};

}

namespace jflinux {

LinuxSpecialFDSuite::LinuxSpecialFDSuite()
: jf::unittest::TestSuite("LinuxSpecialFD")
{
    add_test(new EventFDTest);
    add_test(new TimerFDTest);
    add_test(new SignalFDTest);
}

}
