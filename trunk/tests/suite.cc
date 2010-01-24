// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jflinux/tests/common_suite.h>
#include <jflinux/tests/pthread_suite.h>
#include <jflinux/tests/io_suite.h>
#include <jflinux/tests/debug_suite.h>

// CONFIX:REQUIRE_H('jflinux/tests/common_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/tests/pthread_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/tests/io_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/tests/debug_suite.h', REQUIRED)

namespace jflinux {

Suite::Suite()
: jf::unittest::TestSuite("LinuxTools")
{
    add_test(new jflinux::CommonSuite);
    add_test(new jflinux::PThreadSuite);
    add_test(new jflinux::IOSuite);
    add_test(new jflinux::DebugSuite);
}

}
