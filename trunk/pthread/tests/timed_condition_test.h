// -*- mode: C++; c-basic-offset: 4 -*-

#ifndef HAVE_JF_UNIX_TOOLS_TESTS_TIMED_CONDITION_TEST_H
#define HAVE_JF_UNIX_TOOLS_TESTS_TIMED_CONDITION_TEST_H

#include <jf/unittest/test_case.h>

namespace jf {
namespace unix_tools {
namespace tests {

class TimedConditionTest : public jf::unittest::TestCase
{
public:
    TimedConditionTest() : jf::unittest::TestCase("jf::unittest::tests::TimedConditionTest") {}
    virtual void run();
};

}
}
}

#endif
