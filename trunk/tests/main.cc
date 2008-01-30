// -*- mode: C++; c-basic-offset: 4 -*-

#include "suite.h"

#include <jf/unittest/tree_test_runner.h>

int main()
{
    jf::unix_tools::tests::Suite suite;
    jf::unittest::TreeTestRunner runner;
    return runner.run(&suite)? 0: 1;
}
