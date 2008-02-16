// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jflinux/pthread/tests/suite.h>
#include <jflinux/io/tests/suite.h>
#include <jflinux/debug/tests/suite.h>

// CONFIX:REQUIRE_H('jflinux/pthread/tests/suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/io/tests/suite.h', REQUIRED)
// CONFIX:REQUIRE_H('jflinux/debug/tests/suite.h', REQUIRED)

namespace jflinux {
namespace tests {

Suite::Suite()
: jf::unittest::TestSuite("jflinux::Suite")
{
    add_test(new jflinux::pthread::tests::Suite);
    add_test(new jflinux::io::tests::Suite);
    add_test(new jflinux::debug::tests::Suite);
}

}
}
