// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef HAVE_JF_UNIX_TOOLS_TESTS_BASIC_THREAD_TEST_H
#define HAVE_JF_UNIX_TOOLS_TESTS_BASIC_THREAD_TEST_H

#include <jf/unittest/test_case.h>

namespace jf {
namespace unix_tools {
namespace tests {

class BasicThreadTest : public jf::unittest::TestCase
{
public:
    BasicThreadTest() : jf::unittest::TestCase("jf::unittest::tests::BasicThreadTest") {}
    virtual void run();
};

}
}
}

#endif
