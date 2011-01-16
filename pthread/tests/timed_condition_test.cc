// -*- mode: C++; c-basic-offset: 4 -*-

// Copyright (C) 2008-2011 Joerg Faschingbauer

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

#include "timed_condition_test.h"

#include <jf/linuxtools/mutex.h>
#include <jf/linuxtools/condition.h>
#include <jf/linuxtools/timespec.h>

namespace jf {
namespace linuxtools {

void TimedConditionTest::run()
{
    jf::linuxtools::Mutex m;
    jf::linuxtools::Condition c(m);

    jf::linuxtools::TimeSpec now(jf::linuxtools::TimeSpec::now_timeofday());
    jf::linuxtools::TimeSpec until(now + jf::linuxtools::TimeSpec(0, jf::linuxtools::TimeSpec::one_second/4));

    bool timedout = c.timed_wait(until);

    JFUNIT_ASSERT(timedout);
    JFUNIT_ASSERT(jf::linuxtools::TimeSpec::now_timeofday().secs() - now.secs() >= 0.25);
}

}
}
