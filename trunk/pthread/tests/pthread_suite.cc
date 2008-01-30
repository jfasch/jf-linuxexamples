// -*- mode: C++; c-basic-offset: 4 -*-

#include "pthread_suite.h"

#include "basic_thread_test.h"
#include "timed_condition_test.h"
#include "tsd_test.h"

namespace jf {
namespace unix_tools {
namespace tests {

PthreadSuite::PthreadSuite()
: jf::unittest::TestSuite("jf::unix_tools::PthreadSuite")
{
    add_test(new BasicThreadTest);
    add_test(new TimedConditionTest);
    add_test(new ThreadSpecificDataTest);
}

}
}
}
