// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jflinux/tests/pthread_suite.h>
#include <jflinux/tests/stacktrace_suite.h>

namespace jflinux {
namespace tests {

Suite::Suite()
: jf::unittest::TestSuite("jflinux::Suite")
{
    add_test(new PthreadSuite);
    add_test(new StackTraceSuite);
}

}
}
