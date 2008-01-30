// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef HAVE_JF_UNIX_TOOLS_TESTS_TSD_TEST_H
#define HAVE_JF_UNIX_TOOLS_TESTS_TSD_TEST_H

#include <jf/unittest/test_case.h>

namespace jf {
namespace unix_tools {
namespace tests {

class ThreadSpecificDataTest : public jf::unittest::TestCase
{
public:
    ThreadSpecificDataTest() : jf::unittest::TestCase("jf::unittest::tests::ThreadSpecificDataTest") {}
    virtual void run();
};

}
}
}

#endif
