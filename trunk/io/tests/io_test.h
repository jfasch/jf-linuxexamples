// (C) Atronic -*- C++ -*-

#ifndef HAVE_JF_LINUX_IO_TEST_H
#define HAVE_JF_LINUX_IO_TEST_H

#include <jf/unittest/test_case.h>

namespace jflinux {
namespace io {
namespace tests {

class IOTest : public jf::unittest::TestCase
{
public:
    IOTest();
    virtual void run();
};

}
}
}

#endif
