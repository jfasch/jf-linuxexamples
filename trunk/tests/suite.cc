// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jflinux/pthread/tests/suite.h>
#include <jflinux/io/tests/suite.h>
#include <jflinux/stacktrace/tests/suite.h>

namespace jflinux {
namespace stacktrace {
namespace tests {

Suite::Suite()
: jf::unittest::TestSuite("jflinux::Suite")
{
    add_test(new jflinux::pthread::tests::Suite);
    add_test(new jflinux::io::tests::Suite);
    add_test(new jflinux::stacktrace::tests::Suite);
}

}
}
}
