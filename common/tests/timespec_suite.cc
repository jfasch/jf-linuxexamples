// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2009-2010 Joerg Faschingbauer

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

#include "timespec_suite.h"

#include <jf/linuxtools/timespec.h>

#include <jf/unittest/test_case.h>

namespace jf {
namespace linuxtools {

class Arithmetic : public jf::unittest::TestCase
{
public:
    Arithmetic() : jf::unittest::TestCase("Arithmetic") {}
    virtual void run()
    {
        {
            TimeSpec l(/*sec=*/3, /*nsec=*/0);
            TimeSpec r(/*sec=*/1, /*nsec=*/0);
            JFUNIT_ASSERT(l+r==TimeSpec(4,0));
        }
        {
            TimeSpec l(1,1);
            TimeSpec r(1,1);
            JFUNIT_ASSERT(l+r==TimeSpec(2,2));
        }
        {
            TimeSpec l(1,TimeSpec::one_second-1);
            TimeSpec r(1,1);
            JFUNIT_ASSERT(l+r==TimeSpec(3,0));
        }
        {
            TimeSpec l(0,1000);
            TimeSpec r(0,TimeSpec::one_second-500);
            JFUNIT_ASSERT(l+r==TimeSpec(1,500));
        }
        {
            TimeSpec l(1,0);
            TimeSpec r(0,1);
            TimeSpec s = l-r;
            JFUNIT_ASSERT(l-r==TimeSpec(0, TimeSpec::one_second-1));
        }
        {
            TimeSpec l(2,3);
            TimeSpec r(1,3);
            JFUNIT_ASSERT(l-r==TimeSpec(1,0));
        }
        {
            TimeSpec l(2,3);
            TimeSpec r(1,5);
            JFUNIT_ASSERT(l-r==TimeSpec(0, TimeSpec::one_second-2));
        }
    }
};

class Underflow : public jf::unittest::TestCase
{
public:
    Underflow() : jf::unittest::TestCase("UnderFlow") {}
    virtual void run()
    {
        {
            try {
                TimeSpec(1,0) - TimeSpec(2,0);
                JFUNIT_FAIL();
            }
            catch (const TimeSpec::UnderflowError&) {}
        }
        {
            try {
                TimeSpec(2,1) - TimeSpec(2,2);
                JFUNIT_FAIL();
            }
            catch (const TimeSpec::UnderflowError&) {}
        }
    }
};

class Monotonic : public jf::unittest::TestCase
{
public:
    Monotonic() : jf::unittest::TestCase("Monotonic") {}
    virtual void run()
    {
        TimeSpec before = TimeSpec::now_monotonic();
        TimeSpec wait(0, 500);
        ::nanosleep(&wait, NULL);
        TimeSpec after = TimeSpec::now_monotonic();
        JFUNIT_ASSERT(before < after);
    }
};


TimeSpecSuite::TimeSpecSuite()
: jf::unittest::TestSuite("TimeSpec")
{
    add_test(new Arithmetic);
    add_test(new Underflow);
    add_test(new Monotonic);
}

}
}
