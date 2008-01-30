// -*- mode: C++; c-basic-offset: 4 -*-

#include "timed_condition_test.h"

#include <jf/unix_tools/mutex.h>
#include <jf/unix_tools/condition.h>

namespace jf {
namespace unix_tools {
namespace tests {

void TimedConditionTest::run()
{
    jf::unix_tools::Mutex m;
    jf::unix_tools::Condition c(m);

    jf::unix_tools::TimeSpec now(jf::unix_tools::TimeSpec::now());
    jf::unix_tools::TimeSpec until(now + jf::unix_tools::TimeSpec(0, jf::unix_tools::TimeSpec::one_second/4));

    bool timedout = c.timed_wait(until);

    JFUNIT_ASSERT(timedout);
    JFUNIT_ASSERT(jf::unix_tools::TimeSpec::now().secs() - now.secs() >= 0.25);
}

}
}
}
