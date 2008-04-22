// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jflinux/tests/common_suite.h>
#include <jflinux/pthread/tests/suite.h>
#include <jflinux/io/tests/suite.h>
#include <jflinux/debug/tests/suite.h>

// CONFIX:REQUIRE_H('jflinux/tests/common_suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/pthread/tests/suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/io/tests/suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/debug/tests/suite.h', REQUIRED)

namespace jflinux {

Suite::Suite()
: jf::unittest::TestSuite("jflinux::Suite")
{
    add_test(new jflinux::CommonSuite);
    add_test(new jflinux::pthread::Suite);
    add_test(new jflinux::io::Suite);
    add_test(new jflinux::debug::Suite);
}

}
