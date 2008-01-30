// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jf/unix_tools/tests/pthread_suite.h>

namespace jf {
namespace unix_tools {
namespace tests {

Suite::Suite()
: jf::unittest::TestSuite("jf::unix_tools::Suite")
{
    add_test(new PthreadSuite);
}

}
}
}
